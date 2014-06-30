<?php

class WellController extends EntityAPIController {
  public function save($well) {
    drupal_write_record('well', $well);
    field_attach_insert('well', $well);
    module_invoke_all('entity_insert', 'well', $well);
    return $well;
  }

} 