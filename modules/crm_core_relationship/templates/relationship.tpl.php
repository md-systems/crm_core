<?php

/**
 * @file
 * Generic relationship display template
 */
?>
<div id="relationship-<?php print $relationship->relation_type . '-' . $relationship->rid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php if ($view_mode !== 'full'): ?>

  <?php endif; ?>
  <?php if ($view_mode === 'full'): ?>

  <?php endif; ?>
</div>
