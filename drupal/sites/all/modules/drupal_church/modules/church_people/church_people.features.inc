<?php
/**
 * @file
 * church_people.features.inc
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

/**
 * Implements hook_image_default_styles().
 */
function church_people_image_default_styles() {
  $styles = array();

  // Exported image style: admin_church_people_list.
  $styles['admin_church_people_list'] = array(
    'name' => 'admin_church_people_list',
    'effects' => array(
      1 => array(
        'label' => 'Scale',
        'help' => 'Scaling will maintain the aspect-ratio of the original image. If only a single dimension is specified, the other dimension will be calculated.',
        'effect callback' => 'image_scale_effect',
        'dimensions callback' => 'image_scale_dimensions',
        'form callback' => 'image_scale_form',
        'summary theme' => 'image_scale_summary',
        'module' => 'image',
        'name' => 'image_scale',
        'data' => array(
          'width' => 200,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
      2 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 50,
          'height' => 50,
        ),
        'weight' => 2,
      ),
    ),
  );

  return $styles;
}
