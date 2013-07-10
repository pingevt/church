<?php
/**
 * @file
 * test.features.inc
 */

/**
 * Implements hook_default_church_people_type().
 */
function church_people_default_church_people_type() {
  $items = array();
  $items['member'] = entity_import('church_people_type', '{
    "type" : "member",
    "label" : "Member",
    "weight" : 0,
    "description" : "Default member",
    "rdf_mapping" : []
  }');
  return $items;
}
