function buildModeTable(modesData) {
    // Clear the mode table in case there's stuff in it
    $('#modesTable tbody').empty();
    
    // If data was given, use it
    if (arguments.length == 1)
    {
        // Use the JSON data to build the table entries
        $(modesData.modes).each(function () {
            // Check if the mode is the matrix's current mode
            var isCurrentMode = (this.index == modesData.currentMode);
            
            // Create the name and description cells
            var nameCell = $('<td/>').text(this.name);
            var descriptionCell = $('<td/>').text(this.description);
            
            // Create the settings cell, making it clickable to open settings
            // for the mode.
            var settingsCell = $('<td/>').width(100)
                .addClass("settingsCell")
                .on(
                    "click",
                    {index: this.index},
                    function(event) {onSettingsButtonClick(event.data.index);});
            
            var row = $('<tr/>');
            
            // If the row is not the current mode, make name/description cells
            // clickable to change the mode.
            if (!isCurrentMode)
            {
                row.addClass("clickableRow");
                (nameCell, descriptionCell).on(
                    "click",
                    {index: this.index},
                    function(event) {onModeButtonClick(event.data.index);});
            }
            else
            {
                row.addClass("nonClickableRow");
            }
            
            // Add the cells to the row and add the row to the table
            row.append(nameCell, descriptionCell, settingsCell);
            $('#modesTable').append(row);
        }); // end (modesData.modes).each
    }
    // If data wasn't given, get it from /getModes
    else
    {
        $.getJSON('getModes', function(data) {buildModeTable(data);});
    }
} // end buildModeTable

function onModeButtonClick(modeIndex) {
    // Send the request to change the mode and use the returned data to update
    // the modes table.
    $.get(
        "setMode?mode=" + modeIndex,
        function(data) {buildModeTable(data);},
        "json")
        .fail(function() {
            $(document.body).append("Unable to change mode, try again.<br>");
        });
} // end onModeButtonClick

function onSettingsButtonClick(modeIndex) {
    // Remove any existing settings row(s) from the table
    $('.settingsRow').remove();
    
    // Build the settings row for the mode
    var settingsCell = $('<td/>').attr('colspan', 3).text("Loading settings...");
    var settingsRow = $('<tr/>').addClass("settingsRow").append(settingsCell);
    
    // Load the settings for the mode
    $.get(
        "getSettings?mode=" + modeIndex,
        function(data) {buildSettingsCell(settingsCell, data);},
        "json")
        .fail( function() {
            settingsCell.text("Unable to load settings, try again.");
        });
    
    // Find the row for the mode and insert the new row after it
    $('#modesTable >tbody >tr').eq(modeIndex).after(settingsRow);
} // end onSettingsButtonClick

function buildSettingsCell(settingsCell, settingsData)
{
    // Build a form to hold the settings' info and inputs
    var settingsForm = $('<form/>');
    
    $(settingsData.settings).each( function(index, setting) {
        // Write the name, description, and current value
        settingsForm.append('<b>' + setting.name + '</b><br>');
        settingsForm.append(setting.description + '.<br>');
        settingsForm.append('Current value: ' + setting[setting.type] + '<br>');
        
        // Add a field to accept a new value
        settingsForm.append('New value:');
        var settingInput = $('<input>').attr('type', 'text')
            .attr('data-modeIndex', settingsData.mode.index)
            .attr('data-settingIndex', setting.index)
            .attr('data-type', setting.type);
        settingsForm.append(settingInput);
        
        settingsForm.append('<br><br>');
    }); // end (settingsData.settings).each
    
    // Add the submit button
    $('<input>').attr('type', 'submit').attr('value', 'Update Settings')
        //.click(function(event) {submitSettings(settingsForm);})
        .appendTo(settingsForm);
        
    // Set up the form's submit action
    settingsForm.submit( function(event) {
        event.preventDefault();
        
        // Get all the text input fields
        $(settingsForm).find(':text').each(function() {
            // Submit the setting update for each input field that is not empty
            if ($(this).val())
            {
                var requestString ='setSetting?'
                    + 'mode=' + $(this).attr('data-modeIndex') + '&'
                    + 'setting=' + $(this).attr('data-settingIndex') + '&'
                    + $(this).attr('data-type') + '=' + $(this).val();
                $.get(
                    requestString,
                    function() {},
                    "json")
                    .fail( function() {
                        settingsForm.append(
                            '<br>Unable to update settings, try again.');
                    });
            }
        });
        
        return false;
    });
    
    // Put the form in the settings cell
    settingsCell.empty().append(settingsForm);
}