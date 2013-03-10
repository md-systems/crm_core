<?php 
/**
 * @file
 * Theme implementation for contact entity.
 *
 * Available variables:
 * - $content: An array of comment items. Use render($content) to print them all, or
 *   print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 *
 * @see template_preprocess()
 * @see template_preprocess_entity()
 * @see template_process()
 */
?>
<div id="contact-<?php print $type . '-' . $cid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php if ($view_mode !== 'full'): ?>
	  <h2<?php print $title_attributes; ?>><a href="<?php print base_path(); ?>crm-core/contact/<?php print $cid; ?>"><?php print render($contact_data['contact_name']); ?></a></h2>
  <?php endif; ?>
  <?php if ($view_mode === 'full'): ?>
	  <?php print render($contact_data); ?>
  <?php endif; ?>
</div>
