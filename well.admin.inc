<?php

/**
 * Generates the well type editing form.
 */
function well_type_form($form, &$form_state, $well_type, $op = 'edit') {

  if ($op == 'clone') {
    $well_type->label .= ' (cloned)';
    $well_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $well_type->label,
    '#description' => t('The human-readable name of this well type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($well_type->type) ? $well_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $well_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'well_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this well type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($well_type->description) ? $well_type->description : '',
    '#description' => t('Description about the well type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save well type'),
    '#weight' => 40,
  );

  if (!$well_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete well type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('well_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Submit handler for creating/editing well_type.
 */
function well_type_form_submit(&$form, &$form_state) {
  $well_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  well_type_save($well_type);

  // Redirect user back to list of well types.
  $form_state['redirect'] = 'admin/structure/well-types';
}

function well_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/well-types/' . $form_state['well_type']->type . '/delete';
}

/**
 * well type delete form.
 */
function well_type_form_delete_confirm($form, &$form_state, $well_type) {
  $form_state['well_type'] = $well_type;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['well_type_id'] = array('#type' => 'value', '#value' => entity_id('well_type' ,$well_type));
  return confirm_form($form,
    t('Are you sure you want to delete well type %title?', array('%title' => entity_label('well_type', $well_type))),
    'well/' . entity_id('well_type' ,$well_type),
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * well type delete form submit handler.
 */
function well_type_form_delete_confirm_submit($form, &$form_state) {
  $well_type = $form_state['well_type'];
  well_type_delete($well_type);

  watchdog('well_type', '@type: deleted %title.', array('@type' => $well_type->type, '%title' => $well_type->label));
  drupal_set_message(t('@type %title has been deleted.', array('@type' => $well_type->type, '%title' => $well_type->label)));

  $form_state['redirect'] = 'admin/structure/well-types';
}

/**
 * Page to select well Type to add new well.
 */
function well_admin_add_page() {
  $items = array();
  foreach (well_types() as $well_type_key => $well_type) {
    $items[] = l(entity_label('well_type', $well_type), 'well/add/' . $well_type_key);
  }
  return array('list' => array('#theme' => 'item_list', '#items' => $items, '#title' => t('Select type of well to create.')));
}

/**
 * Add new well page callback.
 */
function well_add($type) {
  $well_type = well_types($type);

  $well = entity_create('well', array('type' => $type));
  drupal_set_title(t('Create @name', array('@name' => entity_label('well_type', $well_type))));

  $output = drupal_get_form('well_form', $well);

  return $output;
}

/**
 * well Form.
 */
function well_form($form, &$form_state, $well) {
  $form_state['well'] = $well;

  $form['title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Name'),
    '#default_value' => $well->title,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $well->description,
  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $well->uid,
  );

  field_attach_form('well', $well, $form, $form_state);

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 100,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save well'),
    '#submit' => $submit + array('well_form_submit'),
  );

  // Show Delete button if we edit well.
  $well_id = entity_id('well' ,$well);
  if (!empty($well_id) && well_access('edit', $well)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('well_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'well_form_validate';

  return $form;
}

function well_form_validate($form, &$form_state) {

}

/**
 * well submit handler.
 */
function well_form_submit($form, &$form_state) {
  $well = $form_state['well'];

  entity_form_submit_build_entity('well', $well, $form, $form_state);

  well_save($well);

  $well_uri = entity_uri('well', $well);

  $form_state['redirect'] = $well_uri['path'];

  drupal_set_message(t('well %title saved.', array('%title' => entity_label('well', $well))));
}

function well_form_submit_delete($form, &$form_state) {
  $well = $form_state['well'];
  $well_uri = entity_uri('well', $well);
  $form_state['redirect'] = $well_uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 */
function well_delete_form($form, &$form_state, $well) {
  $form_state['well'] = $well;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['well_type_id'] = array('#type' => 'value', '#value' => entity_id('well' ,$well));
  $well_uri = entity_uri('well', $well);
  return confirm_form($form,
    t('Are you sure you want to delete well %title?', array('%title' => entity_label('well', $well))),
    $well_uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function well_delete_form_submit($form, &$form_state) {
  $well = $form_state['well'];
  well_delete($well);

  drupal_set_message(t('well %title deleted.', array('%title' => entity_label('well', $well))));

  $form_state['redirect'] = '<front>';
}
