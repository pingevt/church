<?php
/**
 * @file
 * Hooks provided by this module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Acts on church_people being loaded from the database.
 *
 * This hook is invoked during $church_people loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of $church_people entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_church_people_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a $church_people is inserted.
 *
 * This hook is invoked after the $church_people is inserted into the database.
 *
 * @param ExampleTask $church_people
 *   The $church_people that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_church_people_insert(ChurchPeople $church_people) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('church_people', $church_people),
      'extra' => print_r($church_people, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a $church_people being inserted or updated.
 *
 * This hook is invoked before the $church_people is saved to the database.
 *
 * @param ExampleTask $church_people
 *   The $church_people that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_church_people_presave(ChurchPeople $church_people) {
  $church_people->name = 'foo';
}

/**
 * Responds to a $church_people being updated.
 *
 * This hook is invoked after the $church_people has been updated in the database.
 *
 * @param ExampleTask $church_people
 *   The $church_people that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_church_people_update(ChurchPeople $church_people) {
  db_update('mytable')
    ->fields(array('extra' => print_r($church_people, TRUE)))
    ->condition('id', entity_id('church_people', $church_people))
    ->execute();
}

/**
 * Responds to $church_people deletion.
 *
 * This hook is invoked after the $church_people has been removed from the database.
 *
 * @param ExampleTask $church_people
 *   The $church_people that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_church_people_delete(ChurchPeople $church_people) {
  db_delete('mytable')
    ->condition('pid', entity_id('church_people', $church_people))
    ->execute();
}

/**
 * Act on a church_people that is being assembled before rendering.
 *
 * @param $church_people
 *   The church_people entity.
 * @param $view_mode
 *   The view mode the church_people is rendered in.
 * @param $langcode
 *   The language code used for rendering.
 *
 * The module may add elements to $church_people->content prior to rendering. The
 * structure of $church_people->content is a renderable array as expected by
 * drupal_render().
 *
 * @see hook_entity_prepare_view()
 * @see hook_entity_view()
 */
function hook_church_people_view($church_people, $view_mode, $langcode) {
  $church_people->content['my_additional_field'] = array(
    '#markup' => $additional_field,
    '#weight' => 10,
    '#theme' => 'mymodule_my_additional_field',
  );
}

/**
 * Alter the results of entity_view() for church_peoples.
 *
 * @param $build
 *   A renderable array representing the church_people content.
 *
 * This hook is called after the content has been assembled in a structured
 * array and may be used for doing processing which requires that the complete
 * church_people content structure has been built.
 *
 * If the module wishes to act on the rendered HTML of the church_people rather than
 * the structured content array, it may use this hook to add a #post_render
 * callback. Alternatively, it could also implement hook_preprocess_church_people().
 * See drupal_render() and theme() documentation respectively for details.
 *
 * @see hook_entity_view_alter()
 */
function hook_church_people_view_alter($build) {
  if ($build['#view_mode'] == 'full' && isset($build['an_additional_field'])) {
    // Change its weight.
    $build['an_additional_field']['#weight'] = -10;

    // Add a #post_render callback to act on the rendered HTML of the entity.
    $build['#post_render'][] = 'my_module_post_render';
  }
}

/**
 * Acts on church_people_type being loaded from the database.
 *
 * This hook is invoked during church_people_type loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of church_people_type entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_church_people_type_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a church_people_type is inserted.
 *
 * This hook is invoked after the church_people_type is inserted into the database.
 *
 * @param ExampleTaskType $church_people_type
 *   The church_people_type that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_church_people_type_insert(ChurchPeopleType $church_people_type) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('church_people_type', $church_people_type),
      'extra' => print_r($church_people_type, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a church_people_type being inserted or updated.
 *
 * This hook is invoked before the church_people_type is saved to the database.
 *
 * @param ExampleTaskType $church_people_type
 *   The church_people_type that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_church_people_type_presave(ChurchPeopleType $church_people_type) {
  $church_people_type->name = 'foo';
}

/**
 * Responds to a church_people_type being updated.
 *
 * This hook is invoked after the church_people_type has been updated in the database.
 *
 * @param ExampleTaskType $church_people_type
 *   The church_people_type that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_church_people_type_update(ChurchPeopleType $church_people_type) {
  db_update('mytable')
    ->fields(array('extra' => print_r($church_people_type, TRUE)))
    ->condition('id', entity_id('church_people_type', $church_people_type))
    ->execute();
}

/**
 * Responds to church_people_type deletion.
 *
 * This hook is invoked after the church_people_type has been removed from the database.
 *
 * @param ExampleTaskType $church_people_type
 *   The church_people_type that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_church_people_type_delete(ChurchPeopleType $church_people_type) {
  db_delete('mytable')
    ->condition('pid', entity_id('church_people_type', $church_people_type))
    ->execute();
}

/**
 * Define default church_people_type configurations.
 *
 * @return
 *   An array of default church_people_type, keyed by machine names.
 *
 * @see hook_default_church_people_type_alter()
 */
function hook_default_church_people_type() {
  $defaults['main'] = entity_create('church_people_type', array(
    // …
  ));
  return $defaults;
}

/**
 * Alter default church_people_type configurations.
 *
 * @param array $defaults
 *   An array of default church_people_type, keyed by machine names.
 *
 * @see hook_default_church_people_type()
 */
function hook_default_church_people_type_alter(array &$defaults) {
  $defaults['main']->name = 'custom name';
}
