Visual Studio - To run AutoTester directly from visual Studio

1. In the Code_A0123456Y folder, open the StartupSPASolution.sln.

2. In the solution explorer, right-click "AutoTester" and click on "properties".

3. In the Configuration Properties, click on "Debugging"

4. Change the "Command Arguments" accordingly and click "OK" after change it

"..\Tests\Sample_source.txt ..\Tests\Sample_queries.txt ..\Tests\out.xml"

5. In the solution explorer, right click AutoTester and click on "set as startup project".

6. In the build tab, click on "Build Solution".

7. Run the local window debugger.

9. The output file is located in the "Test" folder.

10. Open out.xml with firefox. 

If you encounter any error. Please visit about:config in firefox address bar and change the preference "privacy.file_unique_origin" to false

If the preference do not have "privacy.file_unique_origin", use this instead "security.fileuri.strict_origin_policy" to false.