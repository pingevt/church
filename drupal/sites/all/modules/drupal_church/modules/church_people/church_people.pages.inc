<?php

/**
 * Task view callback.
 */
function church_people_view($church_people) {
  drupal_set_title(entity_label('church_people', $church_people));
  return entity_view('church_people', array(entity_id('church_people', $church_people) => $church_people), 'full');
}
