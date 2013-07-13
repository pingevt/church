<?php

/**
 * @file
 * drupal_church entityreference autocomplete functionality.
 *
 * copied and modified from entityreference_autocomplete module
 */

function drupal_church_era_bundles_load($bundles_string) {
  return ($bundles_string == '*') ? NULL : explode('+', $bundles_string);
}

function drupal_church_era_column_load($col_string) {
  return ($col_string == '*') ? NULL : $col_string;
}


/**
 * Implements hook_element_info().
 */
function drupal_church_element_info() {
  $types['drupal_church_entityreference'] = array(
    '#input' => TRUE,
    '#size' => 60,
    '#maxlength' => 128,
    '#autocomplete_path' => FALSE,
    '#process' => array('ajax_process_form', 'drupal_church_era_process_entityreference'),
    '#element_validate' => array('drupal_church_validate_era'),
    '#theme' => 'textfield',
    '#theme_wrappers' => array('form_element'),
    // Custom attributes. Set to false so that nothing happens if a dev doesn't
    // specify them.
    '#era_entity_type' => FALSE,
    '#era_bundles' => FALSE,
    '#era_cardinality' => 1,
    '#era_column' => FALSE,
  );
  return $types;
}

/**
 * Form element processing handler for the 'drupal_church_entityreference' element type.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 *
 * @return
 *   The processed element.
 *
 * @see drupal_church_element_info()
 */
function drupal_church_era_process_entityreference($element, &$form_state) {
  if ($element['#era_entity_type']) {
    // Set the autocomplete path based on the entity type to look for.
    $autocomplete_path = 'admin/church/autocomplete/' . $element['#era_entity_type'];

    //add column
    if (!empty($element['#era_column']) && is_string($element['#era_column'])) {
      $autocomplete_path .= '/' . $element['#era_column'];
    }
    else {
      $autocomplete_path .= '/*';
    }

    // If there's bundle filtering, add it. Otherwise, set a wildcard. Also,
    // force the property to be an array.
    if (!empty($element['#era_bundles']) && is_array($element['#era_bundles'])) {
      $autocomplete_path .= '/' . implode('+', $element['#era_bundles']);
    }
    else {
      $autocomplete_path .= '/*';
    }

    $element['#autocomplete_path'] = $autocomplete_path;
  }
  return $element;
}

/**
 * Page callback for the entityreference elements autocomplete path.
 */
function drupal_church_era_callback($entity_type, $column, $bundles, $string = '') {
watchdog('drupal_church', $entity_type);
watchdog('drupal_church', $column);
watchdog('drupal_church', $bundles);
watchdog('drupal_church', $string);

  $matches = array();

  // The user enters a comma-separated list of entity labels. Autocomplete only
  // the last label.
  $labels_typed = drupal_church_explode_tags($string);
  $last_label = drupal_strtolower(array_pop($labels_typed));
  if (!empty($last_label)) {
    $prefix = count($labels_typed) ? implode(', ', $labels_typed) . ', ' : '';
  }

  if (isset($last_label)) {
    // Get entity metadata, to be used for some checks.
    $entity_info = entity_get_info($entity_type);

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $entity_type);

    // If $bundless is not null, add the 'bundle' condition. Also, save some
    // pain to anyone trying to specify a bundle when the entity has no actual
    // key for bundles.
    if ($bundles  && !empty($entity_info['entity keys']['bundle'])) {
      $query->entityCondition('bundle', $bundles);
    }

    // Need to fetch the column used as entity label.
    if ($column) {
      $label_column = $column;
    }
    elseif (isset($entity_info['entity keys']['label'])) {
      $label_column = $entity_info['entity keys']['label'];
    }
    // Interestingly that Drupal's core doesn't define any label for users, is
    // that considered a core bug? TODO: Move this to a more proper place.
    else {
      switch ($entity_type) {
        case 'user':
          $label_column = 'name';
          break;
      }
    }

    $query->propertyCondition($label_column, '%' . $last_label . '%', 'LIKE');
    $result = $query->execute();

    if (!empty($result[$entity_type])) {
      $entities = entity_load($entity_type, array_keys($result[$entity_type]));
      foreach ($entities as $entity_id => $entity) {
        if (entity_access('view', $entity_type, $entity)) {
          $key = $option = check_plain(entity_label($entity_type, $entity)) . ' (' . $entity_id . ')';
          // Names containing commas or quotes must be wrapped in quotes.
          if (strpos($key, ',') !== FALSE || strpos($key, '"') !== FALSE) {
            $key = '"' . str_replace('"', '""', $key) . '"';
          }

          $matches[$prefix . $key] = $option;
        }
      }
    }
  }
  drupal_json_output($matches);
}

/**
 * Form element validation handler for drupal_church_entityreference elements.
 *
 * Note that #required is validated by core.
 */
function drupal_church_validate_era(&$element, &$form_state) {
  $labels_string = $element['#value'];
  $entity_labels = drupal_explode_tags($labels_string);
  $values = array();
  $error = FALSE;

  // If there are more values than the allowed, set an error and return. No need
  // to validate each of the values.
  if (count($entity_labels) > $element['#era_cardinality']) {
    form_error($element, t('The "!field" field can\'t contain more than !cardinality !format_plural_values.',
      array(
        '!field' => t($element['#title']),
        '!cardinality' => $element['#era_cardinality'],
        '!format_plural_values' => format_plural($element['#era_cardinality'], 'value', 'values')
      )));
    return;
  }

  foreach ($entity_labels as $label) {
    if (preg_match("/.+\((\d+)\)/", $label, $matches)) {
      $entity_id = $matches[1];

      // Load the referenced entity.
      $entity = entity_load_single($element['#era_entity_type'], $entity_id);
      list(,, $bundle) = entity_extract_ids($element['#era_entity_type'], $entity);

      // If the value entered doesn't reference an entity, or has been badly
      // entered, trigger an error.
      if (!$entity) {
        $error = TRUE;
        break;
      }
      // Value entered properly, and refers to an existing entity.
      else {
        // User doesn't have read (view) access to it, set an error.
        if (!entity_access('view', $element['#era_entity_type'], $entity)) {
          $error = TRUE;
          break;
        }
        // If a specific bundle type was specified.
        else if ($element['#era_bundles']) {
          // Invalid bundle, set an error.
          if ($element['#era_bundles'] !== $bundle) {
            $error = TRUE;
            break;
          }
        }

        // Finally, check if the entity label is the same as the one entered by the user.
        list($entity_label)  = preg_split("/\((\d+)\)$/", $label);
        if (entity_label($element['#era_entity_type'], $entity) !== trim($entity_label)) {
          $error = TRUE;
          break;
        }
      }

      // Return some basic context of the entity referenced by the user.
      $values[] = array(
        'entity_id' => $entity_id,
        'entity_label' => entity_label($element['#era_entity_type'], $entity),
        'entity_type' => $element['#era_entity_type'],
        'entity_bundle' => $bundle,
      );
    }
    else {
      $error = TRUE;
      break;
    }
  }
  // If an error has been triggered, display it on the form.
  if ($error) {
    form_error($element, t('The "!field" field contains an invalid value.', array('!field' => t($element['#title']))));
  }
  // No errors, so update the value of the entityreference element.
  else {
    // If there's only one allowed, return values into the element's index.
    $values = ($element['#era_cardinality'] === 1) ? current($values) : $values;
    form_set_value($element, $values, $form_state);
  }
}

/**
 * Explodes a string of tags into an array.
 *
 * This is almost a clone of drupal's drupal_explode_tags() function. The reason
 * to use this custom function is because drupal_explode_tags() assumes that the
 * tags entered will be saved straight into the database after being returned,
 * so it removes any escape formatting of the tags. That will make the
 * autocomplete callback to remove any quotes added to tags when users add a
 * different tag, making the escaping of previous ones to disappear, which
 * ultimately will result in an invalid value error in the validation callback.
 *
 * This function does the same as drupal_explode_tags() in terms of exploding
 * the tags, but it doesn't remove any escape formatting from the tags to be
 * returned.
 *
 * @see drupal_explode_tags()
 */
function drupal_church_explode_tags($tags) {
  $regexp = '%(?:^|,\ *)("(?>[^"]*)(?>""[^"]* )*"|(?: [^",]*))%x';
  preg_match_all($regexp, $tags, $matches);
  $typed_tags = array_unique($matches[1]);
  return $typed_tags;
}