<?php

/**
 * Implements hook_views_data()
 */
function church_people_views_data_alter(&$data) {
  $data['church_people']['link_church_people'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the church_people.'),
      'handler' => 'church_people_handler_link_field',
    ),
  );
  $data['church_people']['edit_church_people'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the church_people.'),
      'handler' => 'church_people_handler_edit_link_field',
    ),
  );
  $data['church_people']['delete_church_people'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the church_people.'),
      'handler' => 'church_people_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows models/model/%model_id/op
  $data['church_people']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this church_people.'),
      'handler' => 'church_people_handler_church_people_operations_field',
    ),
  );
}
