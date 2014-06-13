<?php
/**
 * @file
 * Contains \Drupal\crm_core_submission\Plugin\rest\resource\SubmissionResource.
 */

namespace Drupal\crm_core_submission\Plugin\rest\resource;

use Drupal\crm_core_submission\Submission;
use Drupal\rest\Plugin\ResourceBase;
use Drupal\rest\ResourceResponse;

/**
 * Provides a resource for collecting crm submissions.
 *
 * @RestResource(
 *   id = "crm_core_submission",
 *   label = @Translation("CRM Submission"),
 *   serialization_class = "Drupal\crm_core_submission\Submission",
 *   uri_paths = {
 *     "http://drupal.org/link-relations/create" = "/crm-core/api/v1/submissions"
 *   }
 * )
 */
class SubmissionResource extends ResourceBase {

  /**
   * Responds to submission POST requests and echos the data.
   *
   * @param \Drupal\crm_core_submission\Submission $submission
   *   The submission..
   *
   * @return \Drupal\rest\ResourceResponse
   *   The HTTP response object.
   *
   * @todo Do some real stuff here.
   * Save the submitted data an trigger queues so this data can be analyzed and
   * linked with CRM data.
   */
  public function post(Submission $submission) {
    return new ResourceResponse($submission);
  }
}
