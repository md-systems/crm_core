<?php 


/**
 * Class for evaluating name fields.
 */
class nameMatchField extends CrmCoreMatchField{
  
  
  
  public function fieldRender($formElement, $formInfo){
    
    dpm($formElement);
    dpm($formInfo);
    
    
    /*
     * Need to add a set of form elements
     * Should only add the form elements that are enabled for the contact type
     * The structure will look like this...
     * 
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
      '#markup' => $field_info['type'],
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
    */
    
  }
  
  public function operators(){
  }
  
  public function fieldQuery(){
  }
  
  
}
