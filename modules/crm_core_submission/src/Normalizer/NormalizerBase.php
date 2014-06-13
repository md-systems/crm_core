<?php
/**
 * @file
 * Contains \Drupal\crm_core_submission\Normalizer\NormalizerBase.
 *
 * @todo Move to a multipart module similar to hal.
 */

namespace Drupal\crm_core_submission\Normalizer;

use Drupal\serialization\Normalizer\NormalizerBase as SerializationNormalizerBase;
use Symfony\Component\Serializer\Normalizer\DenormalizerInterface;

class NormalizerBase extends SerializationNormalizerBase implements DenormalizerInterface {

  /**
   * The interface or class that this Normalizer supports.
   *
   * @var string|array
   */
  protected $supportedInterfaceOrClass = 'Drupal\crm_core_submission\MultipartInterface';

  /**
   * The formats that the Normalizer can handle.
   *
   * @var array
   */
  protected $formats = array('multipart_mixed');

  /**
   * {@inheritdoc}
   */
  public function denormalize($data, $class, $format = NULL, array $context = array()) {
    /* @var \Drupal\crm_core_submission\MultipartInterface $object */
    $object = new $class();
    if (is_array($data)) {
      foreach ($data as $key => $value) {
        $object->setPart($key, $value);
      }
    }
    else {
      $object->setPart('data', $data);
    }

    return $object;
  }

  /**
   * {@inheritdoc}
   */
  public function normalize($object, $format = NULL, array $context = array()) {
    /* @var \Drupal\crm_core_submission\MultipartInterface $object */
    return $object->getParts();
  }
}
