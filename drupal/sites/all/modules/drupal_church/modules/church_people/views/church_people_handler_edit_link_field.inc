<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields
 */


class church_people_handler_edit_link_field extends church_people_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};

    //Creating a dummy church_people to check access against
    $dummy_church_people = (object) array('type' => $type);
    if (!church_people_access('edit', $dummy_church_people)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $church_people_id = $values->{$this->aliases['cpid']};

    return l($text, 'admin/church/configure/people/' . $church_people_id . '/edit');
  }
}
