<?php 


/**
 * Class for evaluating text fields.
 */
class textMatchField extends CrmCoreMatchField{
  
  /**
   * Constructs the UI for configuring matching on text fields
   * 
   * @see CrmCoreMatchField::fieldRender()
   */
  
  public function fieldRender($field, $field_info, &$form){
    
    $field_name = $field['field_name'];
    $field_label = $field['label'];
    $contact_type = $field['bundle'];

    $config = _crm_core_default_matching_engine_load_field_config($contact_type, $field_name);

    $form['field_matching'][$field_name]['status'] = array(
      '#type' => 'checkbox',
      '#default_value' => $config['status'],
    );

    $form['field_matching'][$field_name]['name'] = array(
      '#type' => 'item',
      '#markup' => $field_label,
    );

    $form['field_matching'][$field_name]['field_name'] = array(
      '#type' => 'item',
      '#markup' => $field_info['type'],
    );

    // @TODO: add a selector for the logical operator
    $form['field_matching'][$field_name]['operator'] = array(
      '#type' => 'select',
      '#default_value' => $config['operator'],
      '#options' => $this->operators(),
    );
    
    // @TODO: add a textfield for any extra information that's needed
    $form['field_matching'][$field_name]['options'] = array(
      '#type' => 'item',
      '#markup' => '---',
    );
    
    $form['field_matching'][$field_name]['score'] = array(
      '#type' => 'textfield',
      '#maxlength' => 28,
      '#size' => 28,
      '#default_value' => $config['score'],
    );
    
    $form['field_matching'][$field_name]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $config['weight'],
      '#attributes' => array(
        'class' => array('crm-core-match-engine-order-weight'),
      ),
    );    
  }
  
  public function operators(){
    // @TODO: add operators for EQUALS, STARTS WITH, ENDS WITH.
    // This is placeholder for debug purposes.
    return array(
      'equals' => t('Equals'),
      'starts' => t('Starts with'),
      'ends' => t('Ends with'),
    );
  }
  
  public function fieldQuery(){
  }
}
