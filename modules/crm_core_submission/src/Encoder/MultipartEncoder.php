<?php

/**
 * @file
 * Contains \Drupal\crm_core_submission\MultipartEncoder.
 *
 * @todo Move to a multipart module similar to hal.
 */

namespace Drupal\crm_core_submission\Encoder;

use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\Serializer\Encoder\DecoderInterface;
use Symfony\Component\Serializer\Encoder\EncoderInterface;

/**
 * Encodes multipart data into a multipart body.
 *
 * Simply respond to multipart/mixed requests using appropriate encoders for
 * message part.
 */
class MultipartEncoder implements  EncoderInterface, DecoderInterface {

  /**
   * The request object.
   *
   * @var \Symfony\Component\HttpFoundation\Request
   */
  protected $request;

  /**
   * The formats that this Encoder supports.
   *
   * @var string
   */
  protected $format = 'multipart_mixed';

  /**
   * Constructs a Multipart encoder.
   *
   * @todo Investigate request injection.
   * Getting the request object via injection might have unpredictable side
   * effects.
   */
  public function __construct(Request $request) {
    $this->request = $request;
  }

  /**
   * {@inheritdoc}
   */
  public function supportsEncoding($format) {
    return $format == $this->format;
  }

  /**
   * {@inheritdoc}
   */
  public function supportsDecoding($format) {
    return $format == $this->format;
  }

  /**
   * {@inheritdoc}
   *
   * @todo Consider to make the Content-* headers available in the decoded data.
   */
  public function decode($data, $format, array $context = array()) {
    $parts = array();

    $content_type = $this->request->headers->get('Content-Type');
    // Grab multipart boundary from content type header.
    $matches = array();
    preg_match('/boundary="?(.+)["$]/', $content_type, $matches);
    $boundary = $matches[1];

    // Split content by boundary and get rid of preamble and epilogue.
    $content_parts = preg_split("/(\\n+)?-+$boundary(\\n+)?/", $data);
    array_shift($content_parts);
    array_pop($content_parts);

    // Loop over the parts.
    // Extract and remove the Content- headers.
    foreach ($content_parts as $id => $part) {
      if (empty($part)) {
        continue;
      }

      $matches = array();
      preg_match('/content-type: ?(.+)/i', $part, $matches);
      $part_content_type = $matches[1] ?: 'text/plain; charset=us-ascii';

      // @todo How to identify the content parts correctly.
      //
      // Quote from RFC 2046:
      // "NOTE:  Conspicuously missing from the "multipart" type is a notion of
      // structured, related body parts. It is recommended that those wishing
      // to provide more structured or integrated multipart messaging
      // facilities should define subtypes of multipart that are syntactically
      // identical but define relationships between the various parts. For
      // example, subtypes of multipart could be defined that include a
      // distinguished part which in turn is used to specify the relationships
      // between the other parts, probably referring to them by their
      // Content-ID field.  Old implementations will not recognize the new
      // subtype if this approach is used, but will treat it as
      // multipart/mixed and will thus be able to show the user the parts that
      // are recognized"
      $matches = array();
      preg_match('/content-id: (.+)/i', $part, $matches);
      if (empty($matches[1])) {
        preg_match('/content-disposition:.+name="?(\w+)[";]?/i', $part, $matches);
      }
      $part_content_id = $matches[1] ?: $id;

      $part_content = preg_replace('/content-.+(\s+)?/i', '', $part);

      // @todo Pass message parts to appropriate decoders.
      $parts[$part_content_id] = $part_content;
    }

    return $parts;
  }

  /**
   * {@inheritdoc}
   */
  public function encode($data, $format, array $context = array()) {
    // @todo Implement encode() method.
    return '';
  }
}
