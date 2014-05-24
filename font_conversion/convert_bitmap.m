function [line_image] = convert_bitmap(image_name, desc_name, output_name)

% Read bitmap font into binary image
binary_image = imread(image_name);
binary_image = rgb2gray(binary_image);
binary_image = im2bw(binary_image, 0.5);

% Parse XML description file
parsed_xml = parseXML(desc_name);
font_height = str2double(parsed_xml.Attributes(2).Value);
font_size = str2double(parsed_xml.Attributes(3).Value);

% Remove '#text' entries
original_num_entries = size(parsed_xml.Children, 2);
chars = struct( ...
    'code', ' ', ...
    'offset', 0, ...
    'x', 0, ...
    'y', 0, ...
    'width', 0, ...
    'height', 0);
num_entries = 0;
    
for i = 1:original_num_entries
    % Only parse 'Char' nodes
    if (strcmp(parsed_xml.Children(i).Name, 'Char'))
        % Advance character struct array index
        num_entries = num_entries + 1;
        
        % Parse character code
        chars(num_entries).code = ...
            parsed_xml.Children(i).Attributes(1).Value;
        
        % Parse bitmap offset
        offset = parsed_xml.Children(i).Attributes(2).Value;
        [values, count] = sscanf(offset, '%d %d');
        if (count ~= 2)
            continue
        end
        chars(num_entries).offset = values(2);
        
        % Parse bitmap location and size
        rect = parsed_xml.Children(i).Attributes(3).Value;
        [values, count] = sscanf(rect, '%d %d %d %d');
        if (count ~= 4)
            continue
        end
        chars(num_entries).x = values(1) + 1;
        chars(num_entries).y = values(2) + 1;
        chars(num_entries).width = values(3);
        if (chars(num_entries).width == 0)
            chars(num_entries).width = 1;
        end
        chars(num_entries).height = values(4);
        
        % Copy bitmap from original image
        chars(num_entries).image = ...
            false(font_height, chars(num_entries).width);
        source_cols = ...
            chars(num_entries).x + (0:chars(num_entries).width-1);
        source_rows = ...
            chars(num_entries).y + (0:chars(num_entries).height-1);
        char_rows = ...
            chars(num_entries).offset + (0:chars(num_entries).height-1);
        chars(num_entries).image(char_rows,:) = ...
            binary_image(source_rows, source_cols);
        if (~isequal(size(chars(num_entries).image), [font_height, chars(num_entries).width]))
            error('size error')
        end
    end
end
clear parsed_xml original_num_entries

% Create C output and line image
file = fopen(output_name, 'w');
line_image = false(font_size, 1);
col_start = 1 + font_height - font_size;
% Write start of array definition
code_prev = chars(1).code - 1;
%{
fprintf(file, '/* ASCII code of first character, used to convert ASCII code to array index. */\n');
fprintf(file, 'const char FONT_INDEX_START = ''%c'';\n\n', code_prev + 1);
%}
fprintf(file, 'FontChar const font[] = {\n');
for i = 1:num_entries
    % Warning if codes are not sequential
    code_prev = char(code_prev + 1);
    code = chars(i).code;
    if (code ~= code_prev)
        fprintf(2, 'Code order error, ''%c'' -> ''%c''\n', code_prev - 1, ...
            code);
    end
    
    line_image = [line_image, chars(i).image(col_start:end, :), false(font_size, 1)];
    
    % Write the character code and beginning of the data
    if ( (code == '''') || (code == '\') )
        code = sprintf('\\%c', code);
    end
    line = sprintf('    {''%s'', %d,\n', ...
        code, chars(i).width);
    line = sprintf('%s        {', line);
    
    % Write each data value
    for j = 1:chars(i).width
        col_binary = char(chars(i).image(col_start:end,j) + '0')';
        line = sprintf('%s0b%s, ', line, col_binary);
    end
    % Remove trailing ', '
    line = line(1:end-2);
    
    % Write end of line
    if (i ~= num_entries)
        line = sprintf('%s}},\n', line);
    else
        line = sprintf('%s}}\n', line);
    end
    
    fprintf(file, '%s', line);
end
% Write end of array definition
fprintf(file, '};\n');
fclose(file);

end %function convert_bitmap

% XML PARSING CODE COPIED FROM:
% http://www.mathworks.com/help/matlab/ref/xmlread.html

function theStruct = parseXML(filename)
% PARSEXML Convert XML file to a MATLAB structure.
try
   tree = xmlread(filename);
catch
   error('Failed to read XML file %s.',filename);
end

% Recurse over child nodes. This could run into problems 
% with very deeply nested trees.
try
   theStruct = parseChildNodes(tree);
catch
   error('Unable to parse XML file %s.',filename);
end
end


% ----- Local function PARSECHILDNODES -----
function children = parseChildNodes(theNode)
% Recurse over node children.
children = [];
if theNode.hasChildNodes
   childNodes = theNode.getChildNodes;
   numChildNodes = childNodes.getLength;
   allocCell = cell(1, numChildNodes);

   children = struct(             ...
      'Name', allocCell, 'Attributes', allocCell,    ...
      'Data', allocCell, 'Children', allocCell);

    for count = 1:numChildNodes
        theChild = childNodes.item(count-1);
        children(count) = makeStructFromNode(theChild);
    end
end
end

% ----- Local function MAKESTRUCTFROMNODE -----
function nodeStruct = makeStructFromNode(theNode)
% Create structure of node info.

nodeStruct = struct(                        ...
   'Name', char(theNode.getNodeName),       ...
   'Attributes', parseAttributes(theNode),  ...
   'Data', '',                              ...
   'Children', parseChildNodes(theNode));

if any(strcmp(methods(theNode), 'getData'))
   nodeStruct.Data = char(theNode.getData); 
else
   nodeStruct.Data = '';
end
end

% ----- Local function PARSEATTRIBUTES -----
function attributes = parseAttributes(theNode)
% Create attributes structure.

attributes = [];
if theNode.hasAttributes
   theAttributes = theNode.getAttributes;
   numAttributes = theAttributes.getLength;
   allocCell = cell(1, numAttributes);
   attributes = struct('Name', allocCell, 'Value', ...
                       allocCell);

   for count = 1:numAttributes
      attrib = theAttributes.item(count-1);
      attributes(count).Name = char(attrib.getName);
      attributes(count).Value = char(attrib.getValue);
   end
end
end