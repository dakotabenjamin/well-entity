<?php
/**
 * @file
 * Hooks provided by this module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Acts on well being loaded from the database.
 *
 * This hook is invoked during $well loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of $well entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_well_load(array $entities) {
  $result = db_query('SELECT wid, foo FROM {mytable} WHERE wid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->wid]->foo = $record->foo;
  }
}

/**
 * Responds when a $well is inserted.
 *
 * This hook is invoked after the $well is inserted into the database.
 *
 * @param Well $well
 *   The $well that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_well_insert(Well $well) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('well', $well),
      'extra' => print_r($well, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a $well being inserted or updated.
 *
 * This hook is invoked before the $well is saved to the database.
 *
 * @param Well $well
 *   The $well that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_well_presave(Well $well) {
  $well->name = 'foo';
}

/**
 * Responds to a $well being updated.
 *
 * This hook is invoked after the $well has been updated in the database.
 *
 * @param Well $well
 *   The $well that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_well_update(Well $well) {
  db_update('mytable')
    ->fields(array('extra' => print_r($well, TRUE)))
    ->condition('id', entity_id('well', $well))
    ->execute();
}

/**
 * Responds to $well deletion.
 *
 * This hook is invoked after the $well has been removed from the database.
 *
 * @param Well $well
 *   The $well that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_well_delete(Well $well) {
  db_delete('mytable')
    ->condition('wid', entity_id('well', $well))
    ->execute();
}

/**
 * Act on a well that is being assembled before rendering.
 *
 * @param $well
 *   The well entity.
 * @param $view_mode
 *   The view mode the well is rendered in.
 * @param $langcode
 *   The language code used for rendering.
 *
 * The module may add elements to $well->content prior to rendering. The
 * structure of $well->content is a renderable array as expected by
 * drupal_render().
 *
 * @see hook_entity_prepare_view()
 * @see hook_entity_view()
 */
function hook_well_view($well, $view_mode, $langcode) {
  $well->content['my_additional_field'] = array(
    '#markup' => $additional_field,
    '#weight' => 10,
    '#theme' => 'mymodule_my_additional_field',
  );
}

/**
 * Alter the results of entity_view() for wells.
 *
 * @param $build
 *   A renderable array representing the well content.
 *
 * This hook is called after the content has been assembled in a structured
 * array and may be used for doing processing which requires that the complete
 * well content structure has been built.
 *
 * If the module wishes to act on the rendered HTML of the well rather than
 * the structured content array, it may use this hook to add a #post_render
 * callback. Alternatively, it could also implement hook_preprocess_well().
 * See drupal_render() and theme() documentation respectively for details.
 *
 * @see hook_entity_view_alter()
 */
function hook_well_view_alter($build) {
  if ($build['#view_mode'] == 'full' && isset($build['an_additional_field'])) {
    // Change its weight.
    $build['an_additional_field']['#weight'] = -10;

    // Add a #post_render callback to act on the rendered HTML of the entity.
    $build['#post_render'][] = 'my_module_post_render';
  }
}

/**
 * Acts on well_type being loaded from the database.
 *
 * This hook is invoked during well_type loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of well_type entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_well_type_load(array $entities) {
  $result = db_query('SELECT wid, foo FROM {mytable} WHERE wid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->wid]->foo = $record->foo;
  }
}

/**
 * Responds when a well_type is inserted.
 *
 * This hook is invoked after the well_type is inserted into the database.
 *
 * @param WellType $well_type
 *   The well_type that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_well_type_insert(WellType $well_type) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('well_type', $well_type),
      'extra' => print_r($well_type, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a well_type being inserted or updated.
 *
 * This hook is invoked before the well_type is saved to the database.
 *
 * @param WellType $well_type
 *   The well_type that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_well_type_presave(WellType $well_type) {
  $well_type->name = 'foo';
}

/**
 * Responds to a well_type being updated.
 *
 * This hook is invoked after the well_type has been updated in the database.
 *
 * @param WellType $well_type
 *   The well_type that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_well_type_update(WellType $well_type) {
  db_update('mytable')
    ->fields(array('extra' => print_r($well_type, TRUE)))
    ->condition('id', entity_id('well_type', $well_type))
    ->execute();
}

/**
 * Responds to well_type deletion.
 *
 * This hook is invoked after the well_type has been removed from the database.
 *
 * @param WellType $well_type
 *   The well_type that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_well_type_delete(WellType $well_type) {
  db_delete('mytable')
    ->condition('wid', entity_id('well_type', $well_type))
    ->execute();
}

/**
 * Define default well_type configurations.
 *
 * @return
 *   An array of default well_type, keyed by machine names.
 *
 * @see hook_default_well_type_alter()
 */
function hook_default_well_type() {
  $defaults['main'] = entity_create('well_type', array(
    // …
  ));
  return $defaults;
}

/**
 * Alter default well_type configurations.
 *
 * @param array $defaults
 *   An array of default well_type, keyed by machine names.
 *
 * @see hook_default_well_type()
 */
function hook_default_well_type_alter(array &$defaults) {
  $defaults['main']->name = 'custom name';
}
