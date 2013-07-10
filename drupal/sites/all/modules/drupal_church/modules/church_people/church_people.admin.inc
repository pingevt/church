<?php

/**
 * Generates the church_people type editing form.
 */
function church_people_type_form($form, &$form_state, $church_people_type, $op = 'edit') {

  if ($op == 'clone') {
    $church_people_type->label .= ' (cloned)';
    $church_people_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $church_people_type->label,
    '#description' => t('The human-readable name of this church_people type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($church_people_type->type) ? $church_people_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $church_people_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'church_people_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this church_people type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($church_people_type->description) ? $church_people_type->description : '',
    '#description' => t('Description about the church_people type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save people type'),
    '#weight' => 40,
  );

  if (!$church_people_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete people type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('church_people_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Submit handler for creating/editing church_people_type.
 */
function church_people_type_form_submit(&$form, &$form_state) {
  $church_people_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  church_people_type_save($church_people_type);

  // Redirect user back to list of church_people types.
  $form_state['redirect'] = 'admin/church/people/people-types';
}

function church_people_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/church/people/people-types/' . $form_state['church_people_type']->type . '/delete';
}

/**
 * church_people type delete form.
 */
function church_people_type_form_delete_confirm($form, &$form_state, $church_people_type) {
  $form_state['church_people_type'] = $church_people_type;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['church_people_id'] = array('#type' => 'value', '#value' => entity_id('church_people_type' ,$church_people_type));
  return confirm_form($form,
    t('Are you sure you want to delete people type %title?', array('%title' => entity_label('church_people_type', $church_people_type))),
    'admin/church/people/people-types/' . entity_id('church_people_type', $church_people_type),
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * church_people type delete form submit handler.
 */
function church_people_type_form_delete_confirm_submit($form, &$form_state) {
  $church_people_type = $form_state['church_people_type'];
  church_people_type_delete($church_people_type);

  watchdog('church_people_type', '@type: deleted %title.', array('@type' => $church_people_type->type, '%title' => $church_people_type->label));
  drupal_set_message(t('@type %title has been deleted.', array('@type' => $church_people_type->type, '%title' => $church_people_type->label)));

  $form_state['redirect'] = 'admin/church/people/people-types';
}

/**
 * Page to select church_people Type to add new church_people.
 */
function church_people_admin_add_page() {
  $items = array();
  foreach (church_people_types() as $church_people_type_key => $church_people_type) {
    $items[] = l(entity_label('church_people_type', $church_people_type), 'admin/church/people/people-types/' . $church_people_type_key);
  }
  return array('list' => array('#theme' => 'item_list', '#items' => $items, '#title' => t('Select type of people to create.')));
}

/**
 * Add new church_people page callback.
 */
function church_people_add($type) {
  $church_people_type = church_people_types($type);

  $church_people = entity_create('church_people', array('type' => $type));
  drupal_set_title(t('Create @name', array('@name' => entity_label('church_people_type', $church_people_type))));

  $output = drupal_get_form('church_people_form', $church_people);

  return $output;
}

/**
 * church_people Form.
 */
function church_people_form($form, &$form_state, $church_people) {
  $form_state['church_people'] = $church_people;

  $form['title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Name'),
    '#default_value' => $church_people->title,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $church_people->description,
  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $church_people->uid,
  );

  field_attach_form('church_people', $church_people, $form, $form_state);

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 100,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save person'),
    '#submit' => $submit + array('church_people_form_submit'),
  );

  // Show Delete button if we edit church_people.
  $church_people_id = entity_id('church_people' , $church_people);
  if (!empty($church_people_id) && church_people_access('edit', $church_people)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('church_people_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'church_people_form_validate';

  return $form;
}

function church_people_form_validate($form, &$form_state) {

}

/**
 * church_people submit handler.
 */
function church_people_form_submit($form, &$form_state) {
  $church_people = $form_state['church_people'];

  entity_form_submit_build_entity('church_people', $church_people, $form, $form_state);

  church_people_save($church_people);

  $church_people_uri = entity_uri('church_people', $church_people);

  $form_state['redirect'] = $church_people_uri['path'];

  drupal_set_message(t('Person %title saved.', array('%title' => entity_label('church_people', $church_people))));
}

function church_people_form_submit_delete($form, &$form_state) {
  $church_people = $form_state['church_people'];
  $church_people_uri = entity_uri('church_people', $church_people);
  $form_state['redirect'] = $church_people_uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 */
function church_people_delete_form($form, &$form_state, $church_people) {
  $form_state['church_people'] = $church_people;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['church_people_id'] = array('#type' => 'value', '#value' => entity_id('church_people' ,$church_people));
  $church_people_uri = entity_uri('church_people', $church_people);
  return confirm_form($form,
    t('Are you sure you want to delete people %title?', array('%title' => entity_label('church_people', $church_people))),
    $church_people_uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function church_people_delete_form_submit($form, &$form_state) {
  $church_people = $form_state['church_people'];
  church_people_delete($church_people);

  drupal_set_message(t('People %title deleted.', array('%title' => entity_label('church_people', $church_people))));

  $form_state['redirect'] = 'admin/church/people';
}
