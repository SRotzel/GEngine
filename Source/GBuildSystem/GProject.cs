using System.Runtime.InteropServices;
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
            DateTime begin = DateTime.Now;
            GBuilder.build(directory);

            TimeSpan elapsed = begin - DateTime.Now;
            Console.WriteLine("Finished building in " + elapsed);
        }

        public static void projectInfo(string project)
        {
            string projDir = Path.Combine(Directory.GetCurrentDirectory(), project);
            string projFile = Path.Combine(projDir, project + ".GProj");
            if(!File.Exists(projFile))
            {
                Console.WriteLine("[ERROR] couldn't find project " + project);
            }

            string jsonString = File.ReadAllText(projFile);
            var projData = JsonSerializer.Deserialize<ProjFile>(jsonString);

            Console.WriteLine("$ Name: " + projData.Name);
            Console.WriteLine("$ Version " + projData.Version);
            Console.WriteLine($"$ {projData.modules.Count} modules");
        }
    }
}