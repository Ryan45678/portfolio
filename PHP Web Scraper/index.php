<!DOCTYPE html>
<!--
To change this license header, choose License Headers in Project Properties.
To change this template file, choose Tools | Templates
and open the template in the editor.
-->
<html>
    <head>
        <meta charset="UTF-8">
        <title>TxState Faculty Info Scraper</title>
    </head>
    <body>
        <?php
            // ignore unnecessarry warnings that will keep the file from running
            error_reporting(E_ERROR | E_WARNING | E_PARSE);
            
            
            /* iterate through all files in the current directory"*/
            
            // directory object for traversing files in the directory
            $currentDir = new DirectoryIterator(dirname("."));
            // traverse all files in the directory
            foreach($currentDir as $file) 
            {
                // if file is .html, process it. Otherwise, skip it
                if (isHTML($file))
                {
                    // get filename for the current .html file
                    $fname = $file->getFileName();

                    // process the current .html file
                    parsePage($fname);
                }
            }
            
            // report that the files have finished processing
            reportFinished();
            
            /* 
             * returns whether or not a file is .html, given a file
             */
            function isHTML($file)
            {
                // get path info for the file
                $fileInfo = pathinfo($file);
                
                // validate extension, check if it is .html, return results
                if(!empty($fileInfo['extension']) && 
                   $fileInfo['extension'] == "html") 
                {
                    return true;  
                } 
                else
                {
                    return false;
                }
            }
            
            /* 
             * parse the file for the necessary information using regular expressions,
               given a filename 
             */
            function parsePage($fname)
            {
                // set filename used by output .txt file
                $txtFName = setFileName($fname);
                
                // array used to store strings that fit the regular expression
                $match = array();
                
                // read in the .html file as a string
                $htmlFile = fopen($fname, "r") or die("Unable to open " . $fname . " !");
                $contents = fread($htmlFile, filesize($fname));
                fclose($htmlFile);
                
                // initialize variables with their respective labels
                $name = "Name: ";
                $education = "Education: ";
                $researchInterests = "Research Interests: ";
                $email = "Email: ";
                $webpage = "Webpage: ";
                
                /* process file and fill in variables - typically need to specify
                   the shortest match and not case sensitive*/
                
                // process name
                $name .= findMatch("/Science - (.*)title>/si", $contents, $match, 10, 8);
                
                // process education
                $education .= findMatch("/>Education<(.*?)<\/p>/si", $contents, $match, 10, 0);
                preg_replace("/>Education<(.*?)body\">/", "", $education);
                // process research interests
                $researchInterests .= findMatch("/>Research Interests<(.*?)<\/p>/si", $contents, $match, 19, 0);
                preg_replace("/>Research Interests<(.*?)body\"><p>/", "", $researchInterests);
                
                // process email
                $email .= findMatch("/profiles\/(.*?)\//i", $contents, $match, 9, 1);
                
                // process webpage
                $webpage .= findMatch("/href=\"(.*?)\">Homepage/i", $contents, $match, 6, 10);
                
                /* replace multiple spaces with a single space */
                $name = preg_replace("/\s+/", " ", $name);
                $education = preg_replace("/\s+/", " ", $education);
                $researchInterests = preg_replace("/\s+/", " ", $researchInterests);
                $email = preg_replace("/\s+/", " ", $email);
                $webpage = preg_replace("/\s+/", " ", $webpage);
                
                // add newlines to the variables
                $name .= "\n";
                $education .= "\n";
                $researchInterests .= "\n";
                $email .= "@txstate.edu\n";
                $webpage .= "\n";

                // combine the data into a string, ready to be written to .txt file
                $data = $name . $education . $researchInterests . $email . $webpage;
                
                // convert any instance of "&amp;" to "&"
                $data = preg_replace("/&amp;/", "&", $data);
                
                // write the results to a .txt file
                writeResultsToTXT($txtFName, $data);
                return;
            }
            
            /* 
             * Sets filename for the .txt file used as output - mainly to have
             * a cleaner looking file name like "Dr. ____ ____.txt" instead of
             * "Department of Computer Science - Dr. ___ _____.txt".
             */
            function setFileName($fname)
            {
                // set cutoff values for left and right of the filename to be  
                // used in the corresponding .txt filename
                // (shortens to [title] [first] [last])
                $LCutoff = 33;
                $RCutoff = 5;
                
                // file name for the .txt file that will be written to
                return substr($fname, $LCutoff, (strlen($fname) - $RCutoff - $LCutoff)) . ".txt";
            }
            
            /* 
             * finds a matching string given a pattern (regular expression),
             * the source string, a match array for storing matches, and 
             * L and R offset values (to strip extraneous values needed for
             * matching but unwanted in the output).
             */
            function findMatch($pattern, $src, $match, $Loffset, $Roffset)
            {
                // find strings that match the pattern (regular expression)
                preg_match($pattern, $src, $match);
                
                // remove extra spaces
                preg_replace("/\s+/", " ", $match[0]);
                
                // return the matched string with the above modifications,
                // stripping it of any remaining HTML tags and shortening according
                // to the L and R offset values passed to the function
                return strip_tags(substr($match[0], $Loffset, strlen($match[0]) - $Loffset - $Roffset));
            }
            
            /* 
             * write the results to a properly named .txt file, given a .txt
               filename and the data to be written 
             */
            function writeResultsToTXT($txtFName, $data)
            {
                // open the .txt file, write the data, and close the file
                $txtFile = fopen($txtFName, "w") or die("Unable to open " . $txtFName . " !");
                fwrite($txtFile, $data);
                fclose($txtFile);
                return;
            }
            
            /* 
             * reports that the file processing has completed 
             */
            function reportFinished()
            {
                echo "Finished Processing all html files - Look for txt files in 
                    the same directory as the html files.\n txt files are named 
                    according to the name of the corresponding faculty member."; 
            }
        ?>
    </body>
</html>
