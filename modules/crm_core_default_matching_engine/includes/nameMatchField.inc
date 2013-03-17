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
  
  public function fieldRender($formElement, $formInfo, &$form){
    
    // this works
    foreach ($formInfo['columns'] as $item => $info){
      if($formElement['settings']['inline_css'][$item] != 'display:none'){
        
        // construct some fake values to pass to the handler
        // title will be a composite of the field name and the field part
        // i.e. contact_name_given
        
        // @TODO: get the real name of the field in there, contact_field_ is not actually the real name...
        // @TODO: add some extra text to the label to indicate where this came from
        $temp['field_name'] = 'contact_field_' . $item;
        $temp['label'] = $formInfo['settings']['labels'][$item];
        
        if(isset($formElement['settings'][$item . '_field'])){
          // dpm( $item . ': this is a select field');
          $item = new selectMatchField;
          $item->fieldRender($temp, $formInfo, $form);
        } else {
          // dpm( $item . ': this is a text field');
          $item = new textMatchField;
          $item->fieldRender($temp, $formInfo, $form);
        }
        
      }
      
    }

  }
  
  public function operators(){
  }
  
  public function fieldQuery(){
  }
  
  
}
