<?php 


/**
 * Class for evaluating name fields.
 */
class nameMatchField extends CrmCoreMatchField {
  
  /**
   * This function is going to add a number of fields
   * based on what the name field is configured to display.
   * 
   * The name field uses text and select fields to set values
   * we will need to pass information into these other namehandlers
   * to get the right records to pass back
   * 
   * @see CrmCoreMatchField::fieldRender()
   */
  
  public function fieldRender($field, $field_info, &$form){
    foreach ($field_info['columns'] as $item => $info){
      if($field['settings']['inline_css'][$item] != 'display:none'){
        $temp['field_name'] = $field_info['field_name'] . '_' . $item;
        $temp['label'] = $field['label'] . ': ' . $field_info['settings']['labels'][$item];
        $temp['bundle'] = $field['bundle'];
        if(isset($field['settings'][$item . '_field'])){
          $item = new selectMatchField;
          $item->fieldRender($temp, $field_info, $form);
        } else {
          $item = new textMatchField;
          $item->fieldRender($temp, $field_info, $form);
        }
        
      }

    }
  }
  
  public function operators(){
  }
  
  public function fieldQuery(){
  }
  
  
}
