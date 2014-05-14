/**
 * @file
 * A JavaScript file for the theme.
 *
 * In order for this JavaScript to be loaded on pages, see the instructions in
 * the README.txt next to this file.
 */

// JavaScript should be made compatible with libraries other than jQuery by
// wrapping it with an "anonymous closure". See:
// - https://drupal.org/node/1446420
// - http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth
(function ($, Drupal, window, document, undefined) {


// To understand behaviors, see https://drupal.org/node/756722#behaviors
Drupal.behaviors.crm_core_report = {
    attach: function (context) {
        // show tooltips on flot charts
        // check for anything flot
        var charts = $('.flot');
        if(charts.length > 0){
          
          var previousPoint = null, pageX, pageY, followMouse, flotTooltipWidth, flotTooltipHeight, tooltip;
          
          // track the mouse so we always know where to put the tooltip
          $(document).bind('mousemove',function(event){
            pageX = event.pageX;
            pageY = event.pageY;
            if(tooltip && followMouse == true){
              tooltip.css('left',pageX-flotTooltipWidth/2);
              tooltip.css('top',pageY-flotTooltipHeight-30);
            }
          }); 
          
          
          // plothover for the tooltip
          $(".flot").bind("plothover", function(event, pos, item){
            
            // make sure we are over a point
            if (item) {
              
              // make sure tooltips are turned on for the chart
              // if not, this returns nothing
              if(!item.series.show_tooltip){
                return true;
              }

              // give us a cursor when hovering over a point
              document.body.style.cursor = 'pointer';
              
              // remove any existing tooltips
              if(item.dataIndex > 0 && item.dataIndex !== previousPoint){
                // remove any existing tooltips
                $("#flot-tooltip").remove();
              }
              
              if (previousPoint != item.dataIndex) {
                
                var usex, usey, label = '', prefix = '', suffix = '', content = 0;
                
                if(item.pageX){
                  usex = item.pageX;
                  usey = item.pageY-10;
                  followMouse = false;
                } else {
                  usex = pageX;
                  usey = pageY;
                  followMouse = true;
                }
                
                // set the current item to the tooltip
                if(item.dataIndex > 0 && item.dataIndex !== previousPoint){
                  previousPoint = item.dataIndex;
                }                
                
                // remove any existing tooltips
                $("#flot-tooltip").remove();
                
                // set the content
                if(item.series.useLabel == 1){
                  label = '<div class="report-tooltip-label">' + item.series.label + '</div>';
                }
                if(item.series.prefix){
                  prefix = item.series.prefix;
                }
                if(item.series.suffix){
                  suffix = item.series.suffix;
                }
                if(item.series.data[item.dataIndex]){
                  content = label + '<div class="report-tooltip-data">' + prefix + item.series.data[item.dataIndex][1] + suffix + '</div>';
                }
                
                // show the tooltip
                if(content !== ''){
                  showTooltip(usex, usey, content);
                }
                
              }
              
            } else {
              document.body.style.cursor = 'default';
              $("#flot-tooltip").remove();
              previousPoint = null;
            }
          });
        }
        
        // display a tooltip
        function showTooltip(x, y, contents) {
          $('<div id="flot-tooltip">' + contents + '</div>').css( {
              top: y,
              left: x,
          }).appendTo("body").fadeIn(5000);
          tooltip = $('#flot-tooltip');
          flotTooltipHeight = tooltip.height();
          flotTooltipWidth = tooltip.width();
          tooltip.css('top', y - flotTooltipHeight-30);
          tooltip.css('left', x - flotTooltipWidth/2);
        }

    },
};
})(jQuery, Drupal, this, this.document);