using System.Text.Json;

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

    public sealed class ProjFile
    {
        required public string Name { get; set; } = "";
        required public double Version { get; set; }
        required public string WorkingDir { get; set; }
        public bool Debug = false;
        public List<string> modules = [];
    }

    class GProject 
    {
        public static void CreateProj(string projName, bool debug)
        {
            Console.WriteLine($"[{projName}]");
            string cwd = Path.Combine(Directory.GetCurrentDirectory(), projName);
            Console.WriteLine(cwd);
            if(string.IsNullOrWhiteSpace(projName))
            {
                projName = "2DGame";
                Directory.CreateDirectory("2DGame");
            } else 
                Directory.CreateDirectory(projName);
            
            // Create basic files
            Directory.CreateDirectory(Path.Combine(cwd, "Source"));
            Directory.CreateDirectory(Path.Combine(cwd, "Bin"));

            /*string gProjApp = Path.Combine(cwd, "Source", projName + ".h");
            File.WriteAllText(
                gProjApp,
                GTemplateCode.GAppTemplate(projName)
            );*/

            /*string g2DCharacter = Path.Combine(cwd, "Source", "2DCharacter.cpp");
            File.WriteAllText(
                g2DCharacter,
                GTemplateCode.GPlayerTemplateCPP()
            );*/

            GModule.CreateModule(projName, cwd + "/Source");
            GModule.CreateModule("2DCharacter", cwd + "/Source");

            // Create project file
            var projFile = new ProjFile
            {
                Name = projName,
                Version = 1.0,
                WorkingDir = cwd,
                Debug = debug,
                modules = [projName, "2DCharacter"]
            };

            string jsonString = JsonSerializer.Serialize(projFile, new JsonSerializerOptions { WriteIndented = true });
            File.WriteAllText(Path.Combine(Directory.GetCurrentDirectory(), projName, "GApp.GProj"), jsonString);
        }
    }
}