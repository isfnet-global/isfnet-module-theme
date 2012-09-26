<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields
 */


class entityform_handler_edit_link_field extends entityform_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};

    //Creating a dummy entityform to check access against
    $dummy_entityform = (object) array('type' => $type);
    if (!entityform_access('edit', $dummy_entityform)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $entityform_id = $values->{$this->aliases['entityform_id']};

    return l($text, 'entityform/' . $entityform_id . '/edit');
  }
}
