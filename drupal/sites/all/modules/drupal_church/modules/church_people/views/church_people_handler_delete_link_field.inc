<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying deletes links
 * as fields
 */


class church_people_handler_delete_link_field extends church_people_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};

    //Creating a dummy model to check access against
    $dummy_church_people = (object) array('type' => $type);
    if (!church_people_access('edit', $dummy_church_people)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $church_people_id = $values->{$this->aliases['cpid']};

    return l($text, 'admin/church/configure/people/' . $church_people_id . '/delete');
  }
}
