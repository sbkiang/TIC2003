Visual Studio - To run AutoTester directly from visual Studio

1. In the Code_<student number> folder, use Visual Studio 2109 to open the StartupSPASolution.sln.

2. In the solution explorer, right-click "AutoTester" and click on "properties".

3. In the Configuration Properties, click on "All Configurations"

4. Change the "Command Arguments" according the following format, and click "OK" after change it
    4.1 Format: <relative path to source from SPA folder> <relative path to queries from SPA folder> <relative path to output from SPA folder>. Example "..\Tests\iteration1_source.txt ..\Tests\iteration1_queries.txt ..\Tests\iteration1_out.xml"

5. In the solution explorer, right click AutoTester and click on "set as startup project".

6. In the build tab, click on "Build Solution". The compiled execute is located in Code_"A0245304N\Release" if Release build, and "Code_A0245304N\Debug" if Debug build

7. Run the local window debugger.

8. The output file is located in the "Test" folder.

9. Open out.xml with firefox. 

If you encounter any error. Please visit about:config in firefox address bar and change the preference "privacy.file_unique_origin" to false

If the preference do not have "privacy.file_unique_origin", use this instead "security.fileuri.strict_origin_policy" to false.