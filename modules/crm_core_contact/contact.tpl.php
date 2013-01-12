<?php 
  // contact.tpl.php
  // generic contact display template
?>
<div id="contact-<?php print $type . '-' . $cid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php if ($view_mode !== 'full'): ?>
	  <h2<?php print $title_attributes; ?>><a href="<?php print base_path(); ?>crm/contact/<?php print $cid; ?>"><?php print render($contact_data['contact_name']); ?></a></h2>
  <?php endif; ?>
  <?php if ($view_mode === 'full'): ?>
	  <?php print render($contact_data); ?>
  <?php endif; ?>
</div>