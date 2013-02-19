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
<div class="crm_core_reports">
  <?php foreach($report_items as $item): ?>
  <div class="item-info">
  	<?php print $item; ?>
  </div>
  <?php endforeach; ?>
</div>


