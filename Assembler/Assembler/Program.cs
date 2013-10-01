using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Security;

namespace Assembler
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length != 3)
            {
                Console.WriteLine("Incorrect number of arguments. Correct usage:");
                Console.WriteLine("    dcpuasm in out");
                return;
            }
            string inFile = args[1];
            string outFile = args[2];
            List<string> lines = ReadCodeFromFile(inFile);
            if (lines == null)
            {
                return;
            }
            var assembler = new Assembler();
            List<UInt16> code = assembler.Assemble(lines);
            byte[] bytes = ToBytes(code);
            WriteCodeToFile(outFile, bytes);
        }

        static List<string> ReadCodeFromFile(string file)
        {
            try
            {
                return File.ReadLines(file).ToList();
            }
            catch (FileNotFoundException _)
            {
                Console.WriteLine("ERROR: File `" + file + "` does not exist.");
            }
            catch (UnauthorizedAccessException _)
            {
                Console.WriteLine("ERROR: Denied access to file `" + file + "`.");
            }
            catch (SecurityException _)
            {
                Console.WriteLine("ERROR: Security error while reading file `" + file + "`.");
            }
            catch (Exception _)
            {
                Console.WriteLine("ERROR: Unknown error while attempting to read file `" + file + "`.");
            }
            return null;
        }

        static void WriteCodeToFile(string file, byte[] bytes)
        {
            try
            {
                File.WriteAllBytes(file, bytes);
            }
            catch (FileNotFoundException _)
            {
                Console.WriteLine("ERROR: File `" + file + "` does not exist.");
            }
            catch (UnauthorizedAccessException _)
            {
                Console.WriteLine("ERROR: Denied access to file `" + file + "`.");
            }
            catch (SecurityException _)
            {
                Console.WriteLine("ERROR: Security error while writing file `" + file + "`.");
            }
            catch (Exception _)
            {
                Console.WriteLine("ERROR: Unknown error while attempting to write file `" + file + "`.");
            }
        }

        static byte[] ToBytes(List<UInt16> code)
        {
            var bytes = new byte[code.Count * 2];
            for (var i = 0; i < code.Count; i++)
            {
                var word = code[i];
                var upper8 = (word >> 8) & 0xFF;
                var lower8 = word & 0xFF;
                bytes[i * 2] = (byte)upper8;
                bytes[i * 2 + 1] = (byte)lower8;
            }
            return bytes;
        }
    }
}
