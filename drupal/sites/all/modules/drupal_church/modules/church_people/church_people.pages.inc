<?php

/**
 * People view callback.
 */
function church_people_view($entity, $view_mode='full') {

  $entity_type = $entity->entityType();
  $entity_id = entity_id($entity_type, $entity);

  //
  // Remove previously built content, if exists
  //
  $entity->content = array();

  $entity->title = filter_xss($entity->title);

  //
  // Build the fields content
  //
  field_attach_prepare_view($entity_type, array($entity_id => $entity), $view_mode);
  entity_prepare_view($entity_type, array($entity_id => $entity));

  $entity->content += field_attach_view($entity_type, $entity, $view_mode);

   // Specify the theme to use and set the #element. Note that the key
   // you use to pass the entity object must match the key you set in the
   // variables in my_module_theme(). So in the case below, we use the key
   // named #element because in my_module_theme() we set the following code:
   //
   // array(
   //   'my_entity' => array(
   //     'variables' => array('element' => null),
   //     'template' => 'my_entity'
   //   ),
   // );
   //
  $entity->content += array(
    //'#theme'     => $entity_type,
    '#element'   => $entity,
    '#view_mode' => $view_mode,
    '#language'  => LANGUAGE_NONE,
  );

  return $entity->content;




  /*
  drupal_set_title(entity_label('church_people', $church_people));
  return entity_view('church_people', array(entity_id('church_people', $church_people) => $church_people), 'full');
  */
}
