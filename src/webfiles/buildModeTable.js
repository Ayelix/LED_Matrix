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
            
            // Write the name and description cells
            var nameCell = "<td>" + this.name + "</td>"
            var descriptionCell = "<td>" + this.description + "</td>";
            
            var row = $('<tr/>').append(nameCell, descriptionCell);
            
            // If the row is not the current mode, make a click update the mode
            if (!isCurrentMode)
            {
                row.addClass("clickableRow")
                    .on(
                    "click",
                    {index: this.index},
                    function(event) {onModeButtonClick(event.data.index);});
            }
            else
            {
                row.addClass("nonClickableRow");
            }
            
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