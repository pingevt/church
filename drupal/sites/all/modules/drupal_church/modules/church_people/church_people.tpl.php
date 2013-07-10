<?php
  // In a real module variables should be manipulated in a preprocess function.
  $content = $element->content;
?>

<?php
  print render($content['field_name']);
