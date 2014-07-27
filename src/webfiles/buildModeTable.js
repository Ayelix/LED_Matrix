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
    // Clear the settings cell
    settingsCell.empty();
    
    $(settingsData.settings).each( function() {
        settingsCell.append('<b>' + this.name + '</b><br>');
        settingsCell.append(this.description + '<br>');
        settingsCell.append(this.type + '<br><br>');
    }); // end (settingsData.settings).each
}