<?php

/**
 * @file
 * Plugin for ImageX Prev/Next navigation widget as a panel pane.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('ImageX prev/next navigation'),
  'description' => t('Custom prev/next navigation.'),
  'category' => t('ImageX'),
  'edit form' => 'imagex_panels_prev_next_navigation_pane_edit_form',
  'render callback' => 'imagex_panels_prev_next_navigation_pane_render',
  'admin info' => 'imagex_panels_prev_next_navigation_pane_admin_info',
  'defaults' => array(
    'prev' => '< prev',
    'next' => 'next >',
  ),
  'all contexts' => TRUE,
);

/**
 * An edit form for the pane's settings.
 *
 * @TODO: #1 - Create options where user can chose how to order and set prev/next
 *        text using tokens.
 *        #2 - Create option allowing to chose which content types should the
 *        navigation can search through.
 *        #3 - .tpl file allowing html customization.
 *        #4 - If Prev or Next are empty, loop tro it and display the first one
 *        or last one as alternative.
 */
function imagex_panels_prev_next_navigation_pane_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['prev'] = array(
    '#type' => 'select',
    '#title' => t('Select previous link format.'),
    '#options' => array(
      1 => t('< previous'),
      2 => t('< node:title'),
    ),
    '#default_value' => $conf['prev'],
  );

  $form['next'] = array(
    '#type' => 'select',
    '#title' => t('Select previous link format.'),
    '#options' => array(
      1 => t('next >'),
      2 => t('node:title >'),
    ),
    '#default_value' => $conf['next'],
  );

  return $form;
}

/**
 * Submit function, note anything in the formstate[conf] automatically gets saved
 * Notice, the magic that automatically does that for you.
 */
function imagex_panels_prev_next_navigation_pane_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

/**
 * Run-time rendering of the body of the block (content type)
 * See ctools_plugin_examples for more advanced info
 */
function imagex_panels_prev_next_navigation_pane_render($subtype, $conf, $args, $contexts) {
  // Set $data variable to be sent over the render output function.
  if (!empty($contexts)) {
    $data = isset($contexts['panelizer']->data) ? $contexts['panelizer']->data : FALSE;
  }

  $block = new stdClass();
  $block->title = '';
  $block->content = imagex_panels_prev_next_navigation_render_output($data);

  return $block;
}

/**
 * admin info callback for panel pane.
 */
function imagex_panels_prev_next_navigation_pane_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass;
    $block->title = $conf['override_title'] ? $conf['override_title_text'] : t('ImageX prev/next navigation');
//    $block->content = 'BLABALAB';
    return $block;
  }
}

function imagex_panels_prev_next_navigation_render_output($data = NULL) {
  // Some validation.
  if (!$data) {
    return FALSE;
  }

  // Set variables.
  $nid = $data->nid;
  $type = $data->type;

   // Load previous item.
   $previous = db_select('node', 'n')
   ->fields('n',array('nid', 'title'))
   ->condition('n.status', 1)
   ->condition('n.type', $type)
   ->condition('n.nid', $nid,'<')
   ->orderBy('n.nid', 'DESC')
   ->range(0,1)
   ->execute()
   ->fetchAssoc();

   // Load next item.
   $next = db_select('node', 'n')
   ->fields('n',array('nid', 'title'))
   ->condition('n.status', 1)
   ->condition('n.type', $type)
   ->condition('n.created', $nid,'>')
   ->orderBy('n.nid', 'ASC')
   ->range(0,1)
   ->execute()
   ->fetchAssoc();

  // Set item output.
  $html_previous = !empty($previous) ? '<li class="previous">'. l('Previous', url('node/'. $previous['nid'])) .'</li>' : FALSE;
  $html_next = !empty($next) ? '<li class="next">'. l('Next', url('node/'. $next['nid'])) .'</a></li>' : FALSE;

  // Case Prev and Next are not available.
  if (!$html_previous && !$html_next) {
    return FALSE;
  }

  return <<<HTML
    <div class="imagex_panels_prev_next_navigation">
        <ul>
            $html_previous $html_next
        </ul>
    </div>
HTML;
}
