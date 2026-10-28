using System.Runtime.InteropServices;
using System.Text.Json;

namespace GBuildSystem
{
    class GBuilder
    {
        private static void generateDependencies(StreamWriter w, string mod, List<string> pub, List<string> priv)
        {
            w.WriteLine("target_link_libraries(" + mod);
            w.WriteLine("   PUBLIC");
            foreach(string p in pub)
            {
                w.WriteLine(p);
            }
            w.WriteLine("   Private");
            foreach(string p in priv)
            {
                w.WriteLine(p);
            }
            w.WriteLine(")\n");
        }

        private static void generateIncludeDirs(StreamWriter w, string mod, string modDir)
        {
            w.WriteLine("target_include_directories(" + mod);
            w.WriteLine("PUBLIC \"" + modDir + "/Public\"");
            w.WriteLine("PRIVATE \"" + modDir + "/Private\"");
            w.WriteLine(")\n");
        }

        private static void generateLib(StreamWriter w, string mod, string[] sourceFiles)
        {
            w.WriteLine("add_library(" + mod + " STATIC ");
            foreach(string cppFile in sourceFiles)
            {
                w.WriteLine(cppFile + " ");
            }
            w.WriteLine(")");
            w.WriteLine();
        }

        public static void build(string directory = "")
        {
            // Setup
            string cwd = Directory.GetCurrentDirectory();
            string projFile;
            string binDir = Path.Combine(directory, "Bin/");

            if(directory == "")
                projFile = Path.Combine(cwd, ".GProj");   
            else
                projFile = Path.Combine(cwd, directory, directory + ".GProj");

            if(!File.Exists(projFile))
            {
                Console.WriteLine("[ERROR] Project file not found: " + projFile);
                return;
            }
            if(!Directory.Exists(binDir))
                Directory.CreateDirectory(binDir);

            string jsonString = File.ReadAllText(projFile);
            var projFileData = JsonSerializer.Deserialize<ProjFile>(jsonString);
            if(projFileData == null)
            {
                Console.WriteLine("[ERROR] Couldn't read project file: " + projFile);
                return;
            }

            string CMakeListsPath = Path.Combine(binDir, "CMakeLists.txt");
            File.WriteAllText(CMakeListsPath, ""); // Clear the file if it exists
            StreamWriter writer = new StreamWriter(CMakeListsPath, true);
            if(writer == null)
            {
                Console.WriteLine("[ERROR] Couldn't create CMakeLists.txt file: " + CMakeListsPath);
                return;
            }

            writer.WriteLine("cmake_minimum_required(VERSION 3.10)\nproject(" + projFileData.Name + ")\nset(CMAKE_CXX_STANDARD 20)\n");

            // build modules
            int count = 0;
            Console.WriteLine($"Building project: {projFileData.Name} (Version: {projFileData.Version})");
            foreach(string mod in projFileData.modules)
            {
                count++; 
                Console.WriteLine("--------------------------------");

                string modDir = Path.Combine(projFileData.WorkingDir, "Source", mod);
                string modFile = Path.Combine(modDir, mod + ".GModule");
                if(!File.Exists(modFile))
                {
                    Console.WriteLine("[ERROR] Module file not found: " + modFile);
                    return;
                }

                string modJsonString = File.ReadAllText(modFile);
                var modFileData = JsonSerializer.Deserialize<GModuleRules>(modJsonString);
                if(modFileData == null)
                {
                    Console.WriteLine("[ERROR] Couldn't read module file: " + modFile);
                    continue;
                }

                Console.WriteLine($"    [{count}/{projFileData.modules.Count}] Building module: {modFileData.ModuleName} in {modDir}");
                string modHash = GModule.CalculateHash(modDir);
                Console.WriteLine("    Checksum: " + modHash);

                if(modHash.Equals(modFileData.hash))
                {
                    Console.WriteLine($"    => Module {modFileData.ModuleName} is up to date. Skipping build.");
                    continue;
                }
                Console.WriteLine($"    => Module {modFileData.ModuleName} is out of date. Building...");

                string[] cppFiles = Directory.GetFiles(modDir, "Private/*.cpp", SearchOption.AllDirectories);
                if(cppFiles.Length == 0)
                {
                    Console.WriteLine("    No C++ source files found, skipping...");
                    continue;
                }
                Console.WriteLine("    => Found " + cppFiles.Length + " C++ source files");

                generateLib(writer, mod, cppFiles);
                generateIncludeDirs(writer, mod, modDir);
                if(modFileData.PublicDependencies.Count > 0 || modFileData.PrivateDependencies.Count > 0)
                {
                    generateDependencies(writer, mod, modFileData.PublicDependencies, modFileData.PrivateDependencies);
                }
                writer.WriteLine();
            }

            writer.Close();

            if(RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
            {
                try
                {
                    System.Diagnostics.ProcessStartInfo info = new System.Diagnostics.ProcessStartInfo()
                    {
                        FileName = "/bin/bash",
                        Arguments = "cmake --build " + binDir
                    };
                    System.Diagnostics.Process proc  = new System.Diagnostics.Process() { StartInfo = info };
                    proc.Start();
                    Console.WriteLine("\nbuilding to directory " + binDir);
                } catch(Exception e)
                {
                    Console.WriteLine("[ERROR] Couldn't invoke cmake: " + e.Message);
                    return;
                } 
            } else
            {
                Console.WriteLine("[ERROR] unsupported OS");
            }
        }
    }
}