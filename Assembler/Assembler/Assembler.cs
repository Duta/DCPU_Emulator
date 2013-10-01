using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Assembler
{
    class Assembler
    {
        public List<UInt16> Assemble(List<string> rawLines)
        {
            // Ensure standard formatting.
            List<string> formatted = RemoveBlanks(Format(rawLines));
            // If no code entered, take an early exit.
            if (formatted.Count == 0)
            {
                return new List<UInt16>();
            }
            List<Instruction> instructions = Parse(formatted);
            return null;
        }

        private List<Instruction> Parse(List<string> lines)
        {
            foreach (var line in lines)
            {
                
            }
            return null;
        }

        private List<string> Format(List<string> lines)
        {
            var newLines = new List<string>();
            foreach (var line in lines.Where(s => s != null))
            {
                if (line == null)
                {
                    continue;
                }
                string newLine = line.Replace("\\s+", " ").ToUpper();
                newLines.Add(newLine);
            }
            return newLines;
        }

        private List<string> RemoveBlanks(List<string> lines)
        {
            return lines.Where(s => s != null && s.Length > 0).ToList();
        }
    }
}
