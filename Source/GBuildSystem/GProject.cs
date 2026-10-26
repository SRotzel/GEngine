using System.Text.Json;
using System.Text.Json.Serialization;

namespace GBuildSystem
{
    class GTemplateCode  {
        public static string GAppTemplate(string className) => $$"""
        #pragma once
        #include <gEngine/gEngine.h>

        class {{className}} : gApplication {
            {{className}}() : gApplication() {  }

            void start() override {
                Super::start();
            }

            void tick() override {
                Super::tick();
            }

            void end() override {
                Super::end();
            }
        }

        #define G_ENTRY_POINT {{className}}
        #define G_ENGINE_MAIN
        #include <gEngine/gEngineMain.h>
        """;

        public static string GPlayerTemplateCPP() => $$""" 
        #include <gEngine/Player.h>

        class 2DCharacter : gPlayer {
            2DCharacter() : gPlayer() {  }

            void start() override {
                Super::start();
            }

            void tick() override {
                Super::tick();
            }

            void end() override {
                Super::end();
            }
        }
        """;

        public static string GModuleHeaderTemplate(string moduleName) => $$"""
        #pragma once
        #include <Engine/Engine.h>


        class {{moduleName}} {
            {{moduleName}}();

            virtual void start() {}
            virtual void tick()  {}
            virtual void end()   {}
        }
        """;

        public static string GModuleSourceTemplate(string moduleName) => $$"""
        #include "{{moduleName}}.h"

        {{moduleName}}::{{moduleName}}() {
            // Constructor implementation
        }

        {{moduleName}}::start() {
            // Start implementation
        }

        {{moduleName}}::tick() {
            // Tick implementation
        }

        {{moduleName}}::end() {
            // End implementation
        }
        """;
    }

    [JsonSerializable(typeof(ProjFile))]
    public sealed class ProjFile
    {
        required public string Name { get; set; } = "";
        required public double Version { get; set; }
        required public string WorkingDir { get; set; }
        public bool Debug { get; set; } = false;
        public List<string> modules { get; set; } = new List<string>();
    }

    class GProject 
    {
        public static void CreateProject(string projName = "2DGame", bool debug = false)
        {
            Console.WriteLine($"[{projName}]");
            string cwd = Path.Combine(Directory.GetCurrentDirectory(), projName);
            Console.WriteLine(cwd);
            Directory.CreateDirectory(projName);
            
            // Create basic files
            Directory.CreateDirectory(Path.Combine(cwd, "Source"));
            Directory.CreateDirectory(Path.Combine(cwd, "Bin"));

            // Create project file
            var projFile = new ProjFile
            {
                Name = projName,
                Version = 1.0,
                WorkingDir = cwd,
                Debug = debug,
                modules = [projName, "2DCharacter"]
            };
            GModule.CreateModule(projName, cwd + "/Source");
            GModule.CreateModule("2DCharacter", cwd + "/Source");

            string jsonString = JsonSerializer.Serialize(projFile, new JsonSerializerOptions { WriteIndented = true });
            File.WriteAllText(Path.Combine(Directory.GetCurrentDirectory(), projName, projName + ".GProj"), jsonString);
        }

        public static void buildProject(string directory = "")
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

            writer.WriteLine("cmake_minimum_required(VERSION 3.10)\nproject(" + projFileData.Name + ")\nset(CMAKE_CXX_STANDARD 20)\n\n");

            // build modules
            int count = 0;
            Console.WriteLine($"Building project: {projFileData.Name} (Version: {projFileData.Version})");
            foreach(string mod in projFileData.modules)
            {
                Console.WriteLine("--------------------------------");
                count++; 

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
                if(GModule.CalculateHash(modDir) == modFileData.hash)
                {
                    Console.WriteLine($"    Module {modFileData.ModuleName} is up to date. Skipping build.");
                    continue;
                }
                Console.WriteLine($"    Module {modFileData.ModuleName} is out of date. Building...");

                string[] cppFiles = Directory.GetFiles(modDir, "Private/*.cpp", SearchOption.AllDirectories);
                if(cppFiles.Length == 0)
                {
                    Console.WriteLine("    No C++ source files found, skipping");
                    continue;
                }
                Console.WriteLine("    Found " + cppFiles.Length + " C++ source files");

                writer.WriteLine("add_library(" + modFileData.ModuleName + " STATIC ");
                foreach(string cppFile in cppFiles)
                {
                    writer.WriteLine(cppFile + " ");
                }
                writer.WriteLine(")\n");

                if(modFileData.PublicDependencies.Count > 0)
                {
                    writer.WriteLine("target_link_libraries(" + modFileData.ModuleName + "\n");
                    foreach(string pubDep in modFileData.PublicDependencies)
                    {
                        writer.WriteLine(" PUBLIC " + pubDep + "\n");
                    }
                    writer.WriteLine(")\n");
                }

                if(modFileData.PrivateDependencies.Count > 0)
                {
                    writer.WriteLine("target_link_libraries(" + modFileData.ModuleName + "\n");
                    foreach(string pubDep in modFileData.PrivateDependencies)
                    {
                        writer.WriteLine(" PRIVATE " + pubDep + "\n");
                    }
                    writer.WriteLine(")\n");
                }
            }

            writer.Close();
        }
    }
}