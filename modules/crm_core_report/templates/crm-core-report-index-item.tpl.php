<?php

/**
 * @file
 * Default display of reports for CRM Core
 *
 * Available variables:
 * 
 * - $reports: associative array listing all reports available
 *   in the system. Each entry is an array with the following keys:
 *   - title: A title for the report grouping.
 *   - reports: A list of the reports to be found. This is an array
 *     keyed by individual reports, and includes the following keys:
 *     - title: title for the report
 *     - description: a description of the report
 *     - path: a path to the report
 *   - widgets: A list of widgets indexed by CRM Core.
 */
?>
<div class="crm_core_reports">
	<?php 
	  foreach($reports as $item => $listing){
	    print theme('crm_core_report_index_item', array('title' => $listing['title'], 'report' => $listing['reports']));
	  }
	?>
</div>
