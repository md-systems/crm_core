<?php

/**
 * @file
 * Default display of reports for CRM Core
 *
 * Available variables:
 * 
 * - $report: associative array listing reports registered under the 
 *   current grouping.
 *   - title: A title for the report grouping.
 *   - reports: A list of the reports to be found. This is an array
 *     keyed by individual reports, and includes the following keys:
 *     - title: title for the report
 *     - description: a description of the report
 *     - path: a path to the report
 *   - widgets: A list of widgets indexed by CRM Core. These can be ignored
 *     in this template, or used if you want to be funny.
 */

?>
<h3><?php print $title; ?></h3>
<div class="item-info">
	<?php 
    $reports = array();
    foreach($report as $item => $val){
      $reports[] = l(t($val['title']), $val['path']) . '<br>' . t($val['description']);
    }
    if(sizeof($reports) > 0) {
  	  print theme_item_list(array('items' => $reports, 'title' => NULL, 'type' => 'ul', 'attributes' => array())); 
    }
  ?>
</div>


