$(document).ready(function(){
  var MAX_UPLOADING_FILE_SIZE = 524288;

  $("#toolbox textarea").autosize();
  $('#input-file > input').filestyle({ });

  bindTextBookmarks();
  bindRandomExamples();
  bindChangeOutputSize();
  bindChangeOutputType();

  if (isFileReaderSupported) {
    handleUploadingFile(MAX_UPLOADING_FILE_SIZE);
  }

  readPsisOptions();
});

function isFileReaderSupported() {
  if (!window.FileReader) {
    console.log("The file API isn't supported on this browser yet.");
    return false;
  }

  var input = document.getElementsByName('input-file');

  if (!input) {
    console.error("Couldn't find the file input element.");
    return false;
  }

  if (!input[0].files) {
    console.log("This browser doesn't seem to support the `files` property of file inputs.");
    return false;
  }

  return true;
}

function handleUploadingFile(limit) {
  $('#toolbox-form').submit(function(e){

    if (document.getElementById('radio-input-file').checked) {
      var input = document.getElementsByName('input-file')[0];

      if (!input.files[0]) {
        showWarningMessage("Please select a file before clicking 'Go'.");
        return false;
      }
      else if (input.files[0].size > limit) {
        showWarningMessage("File " + input.files[0].name + " with " + input.files[0].size +
                           " bytes in size is too large. Limit is " + limit / 1024 + " KBs." );
        return false;
      }
    }

    return true;
  });
}

function showWarningMessage(msg) {
    $('#output > pre').addClass('alert').text(msg);
}

function bindTextBookmarks() {
  $('#toolbox-text .bookmarks > a').click(function(e){
  e.preventDefault();

    if ($('#input-text').is(':visible')) {
      switchBookmark('text', 'file');
    }
    else {
      switchBookmark('file', 'text');
    }
  });
}

function switchBookmark(from, to) {
  if ($('#input-' + to).is(':visible')) { return; }

  var toggleDuration = 300;

  $('input.input-radio').attr('checked', false);

  $('#input-' + from).fadeOut(toggleDuration, function(){
    $('#input-' + from).hide();
    $('#input-' + to).fadeIn(toggleDuration);
    $('#radio-input-' + to).attr('checked', true);
  });

  $('#bookmark-' + from).fadeOut(toggleDuration, function(){
    $('#bookmark-' + from).hide();
    $('#bookmark-' + to).fadeIn(toggleDuration);
  });

}

function bindRandomExamples() {
  var pipeInput = $('#toolbox-pipe input');
  var textInput = $('#toolbox-text textarea');
  var examples = (pipelineExamples == 'undefined') ? [] : pipelineExamples;

  pipeInput.attr('rel', 'tipsy');
  $('#toolbox-pipe input').tipsy({
      trigger : 'manual',
      gravity : 's'
  });

  $('#random-example').click(function(e){
    e.preventDefault();
    var randomExample = Math.floor(Math.random() * examples.length);

    pipeInput.val(examples[randomExample]['pipe']);
    pipeInput.attr('title', examples[randomExample]['description']);
    pipeInput.tipsy('show');

    textInput.val(examples[randomExample]['text']);
    switchBookmark('file', 'text');

    e.stopPropagation();
    return false;
  });

  $('body').click(function(){
    pipeInput.tipsy('hide');
  });
}

function readPsisOptions() {
  if (psisOptions['isInputFile'] == "on") { switchBookmark('text', 'file'); }

  if (psisOptions['fileToDownload'] != "") {
    $('#output-bottom-info > .download-output').attr('href', psisOptions['fileToDownload']);
  }
  else {
    $('#output-bottom-info').css('display', 'none');
  }

  if (psisOptions['lastOutputType']) { $('#output-type-select').val(psisOptions['lastOutputType']); }
}

function bindChangeOutputSize() {

  if (isOutputType('text')) {
    var query = '#output';
    changeOutputFontSize(query, 8, 20, 2);
  }
  else if (isOutputType('svg')) {
    var query = '#output > svg';
    var width = parseInt($(query).attr('width'));

    changeOutputSVGSize(query,
      width - Math.round(0.75*width), width + Math.round(0.25*width), Math.round(0.1*width)
    );
  }
  else if (isOutputType('image')) {
    var query = '#output > a > img';
    var width = parseInt($(query).width());

    changeOutputImageSize(query,
      width - Math.round(0.75*width), width + Math.round(0.25*width), Math.round(0.1*width)
    );
  }

}

function changeOutputFontSize(query, min, max, step) {
  $('.output-fontsize').click(function() {

    var currentFontSize = parseInt($(query).css('font-size'));
    var currentLineHeight = parseInt($(query).css('line-height'));

    if ($(this).attr('id') == 'increase-output-fontsize' && currentFontSize < max) {
      $(query).css('font-size', (currentFontSize + step) + 'px')
        .css('line-height', (currentLineHeight + step) + 'px');
    }

    if ($(this).attr('id') == 'decrease-output-fontsize' && currentFontSize > min) {
      $(query).css('font-size', (currentFontSize - step) + 'px')
        .css('line-height', (currentLineHeight - step) + 'px');
    }
  });
}

function changeOutputSVGSize(query, min, max, step) {
  $('.output-fontsize').click(function() {
    var currentSVGWidth = parseInt($(query).attr('width'));

    if ($(this).attr('id') == 'increase-output-fontsize' && currentSVGWidth < max) {
      $(query).attr('width', (currentSVGWidth + step) + 'pt');
    }
    if ($(this).attr('id') == 'decrease-output-fontsize' && currentSVGWidth > min) {
      $(query).attr('width', (currentSVGWidth - step) + 'pt');
    }
  });
}

function changeOutputImageSize(query, min, max, step) {
  $('.output-fontsize').click(function() {
    var currentImageWidth = parseInt($(query).width());

    if ($(this).attr('id') == 'increase-output-fontsize' && currentImageWidth < max) {
      $(query).css('width', (currentImageWidth + step) + 'px');
    }
    if ($(this).attr('id') == 'decrease-output-fontsize' && currentImageWidth > min) {
      $(query).css('width', (currentImageWidth - step) + 'px');
    }
  });
}

function isOutputType(type) {
  switch(type) {
    case 'text':
      return $('#output > pre').length ? true : false;
      break;
    case 'svg':
      return $('#output > svg').length ? true : false;
      break;
    case 'image':
      return $('#output > a > img').length ? true : false;
      break;
  }
}

function bindChangeOutputType() {
    var WRITER_PART_REGEXP = ' *(!|\|) +([a-z\-]+-writer|write[a-z\-]*|draw|graph).*';

    $('#select-output-type').change(function() {
        var selectedWriter = $(this).find(':selected').attr('value');

        if (!selectedWriter)
            return;

        var currentPipeline = $('input[name=pipe-text]').val();
        var matchWriter = currentPipeline.match(WRITER_PART_REGEXP);
        var newPipeline = (!matchWriter) ? currentPipeline
                                         : currentPipeline.substring(0, matchWriter.index);
        newPipeline += ' ! ' + selectedWriter;

        $('input[name=pipe-text]').val(newPipeline);
        $('input[name=output-type]').val(selectedWriter);

        $('#toolbox > form').submit();
    });
}
