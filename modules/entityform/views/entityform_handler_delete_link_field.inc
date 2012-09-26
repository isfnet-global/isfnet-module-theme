<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying deletes links
 * as fields
 */


class entityform_handler_delete_link_field extends entityform_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};

    //Creating a dummy entityform to check access against
    $dummy_entityform = (object) array('type' => $type);
    if (!entityform_access('delete', $dummy_entityform)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $entityform_id = $values->{$this->aliases['entityform_id']};

    return l($text, 'entityform/' . $entityform_id . '/delete');
  }
}
