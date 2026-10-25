namespace GBuildSystem
{
    class GBuildSystemCLI
    {
        static void Main(string[] args)
        {
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
                        GProject.CreateProj(args[i + 1], debug);
                        i++;
                        break;
                    case "module":
                        Console.WriteLine("Creating a new module...");
                        GModule.CreateModule(args[i + 1], Directory.GetCurrentDirectory());
                        i++;
                        break;
                    case "build":
                        Console.WriteLine("Building the project...");
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