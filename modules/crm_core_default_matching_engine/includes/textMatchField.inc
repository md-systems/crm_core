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
  
  public function fieldRender($formElement, $formInfo, &$form){
    
    $field_name = $formElement['field_name'];
    $field['label'] = $formElement['label'];

    $form['field_matching'][$field_name]['check'] = array(
      '#type' => 'checkbox',
      '#title' => '',
    );
    $form['field_matching'][$field_name]['name'] = array(
      '#type' => 'item',
      '#markup' => $field['label'],
    );
    $form['field_matching'][$field_name]['fieldtype'] = array(
      '#type' => 'item',
      '#markup' => $formInfo['type'],
    );
    
    // @TODO: add a selector for the logical operator
    $form['field_matching'][$field_name]['operator'] = array(
      '#type' => 'item',
      '#markup' => '---',
    );
    
    // @TODO: add a textfield for any extra information that's needed
    $form['field_matching'][$field_name]['options'] = array(
      '#type' => 'item',
      '#markup' => '---',
    );
    
    // @TODO: add a textfield for any extra information that's needed
    $form['field_matching'][$field_name]['score'] = array(
      '#type' => 'textfield',
      '#title' => 'Score',
      '#maxlength' => 28,
      '#title_display' => 'invisible',
      '#default_value' => 0,
    );
    
    // @TODO: get the actual weight as the default value
    $form['field_matching'][$field_name]['weight'] = array(
      '#type' => 'weight',
      '#title_display' => 'invisible',
      '#default_value' => 0,
      '#attributes' => array(
        'class' => array('crm-core-match-engine-order-weight'),
      ),
    );    
    
  }
  
  public function operators(){
    
    // add operators for EQUALS, STARTS WITH, ENDS WITH
    
  }
  
  public function fieldQuery(){
  }
  
  
}
