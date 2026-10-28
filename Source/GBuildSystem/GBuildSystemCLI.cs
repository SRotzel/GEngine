using System.Text.Json;

namespace GBuildSystem
{

    class GBuildSystemCLI
    {
        const double version = 0.2;
        static void Main(string[] args)
        {
            Console.WriteLine("Version " + version);
            Console.WriteLine("GBuildSystem is running...");
            if (args.Length < 1)
            {
                Console.WriteLine("Missing Arguments! exiting...");
                return;
            }

            bool debug = false;
            for(int i = 0; i < args.Length; i++)
            {
                switch(args[i])
                {
                    case "--debug":
                        debug = true;
                        break;
                    case "new":
                        Console.WriteLine("Creating a new project...");
                        GProject.CreateProject(args[i + 1], debug);
                        i++;
                        break;
                    case "module":
                        Console.WriteLine("Creating a new module...");
                        GModule.CreateModule(args[i + 1], Directory.GetCurrentDirectory());
                        i++;
                        break;
                    case "build":
                        Console.WriteLine("Building project...");
                        if(args.Length > i + 1)
                        {
                            GProject.buildProject(args[i + 1]);
                            i++;
                        } else
                        {
                            GProject.buildProject();
                        }
                        break;
                    case "pinfo":
                        if(args.Length > i + 1)
                            GProject.projectInfo(args[i+1]);
                        else 
                            Console.WriteLine("Missing arguments!");
                        break;
                    case "minfo":
                        if(args.Length > i + 1)
                            GModule.moduleInfo(args[i+1], Directory.GetCurrentDirectory());
                        else 
                            Console.WriteLine("Missing arguments!");
                        break;
                    case "clean":
                        Console.WriteLine("Cleaning the project...");
                        break;
                    case "help":
                        Console.WriteLine("Available commands: new, module, build, clean, help");
                        break;                    
                    default:
                        Console.WriteLine($"Unknown command: {args[i]}");
                        break;
                }
            }
        }
    }
}