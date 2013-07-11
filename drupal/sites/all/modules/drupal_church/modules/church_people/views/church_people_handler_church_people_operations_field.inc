<?php

/**
 * This field handler aggregates operations that can be done on a model
 * under a single field providing a more flexible way to present them in a view
 */
class church_people_handler_church_people_operations_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['cpid'] = 'cpid';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $links = menu_contextual_links('church_people', 'admin/church/people', array($this->get_value($values, 'cpid')));

    if (!empty($links)) {
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
