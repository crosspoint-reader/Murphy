// Writes a compact inventory of a Ghidra program after analysis.

import java.io.File;
import java.io.PrintWriter;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.Symbol;

public class DumpProgramInventory extends GhidraScript {
	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		File out = new File(args.length > 0 ? args[0] : "ghidra_inventory.txt");
		try (PrintWriter pw = new PrintWriter(out)) {
			pw.println("# Ghidra Program Inventory");
			pw.println();
			pw.println("Program: " + currentProgram.getName());
			pw.println("Language: " + currentProgram.getLanguageID());
			pw.println("Compiler: " + currentProgram.getCompilerSpec().getCompilerSpecID());
			pw.println();

			pw.println("## Memory Blocks");
			for (MemoryBlock block : currentProgram.getMemory().getBlocks()) {
				pw.printf("- %s start=%s end=%s size=0x%x R=%s W=%s X=%s%n",
					block.getName(), block.getStart(), block.getEnd(), block.getSize(),
					block.isRead(), block.isWrite(), block.isExecute());
			}
			pw.println();

			pw.println("## Symbols Containing ESP");
			int symbolCount = 0;
			for (Symbol sym : currentProgram.getSymbolTable().getAllSymbols(true)) {
				if (sym.getName().toLowerCase().contains("esp")) {
					pw.printf("- %s %s %s%n", sym.getAddress(), sym.getSymbolType(), sym.getName(true));
					if (++symbolCount >= 200) {
						pw.println("- ... truncated at 200 symbols");
						break;
					}
				}
			}
			pw.println();

			pw.println("## Functions");
			int count = currentProgram.getFunctionManager().getFunctionCount();
			pw.println("Function count: " + count);
			int emitted = 0;
			for (Function fn : currentProgram.getFunctionManager().getFunctions(true)) {
				pw.printf("- %s %s%n", fn.getEntryPoint(), fn.getName(true));
				if (++emitted >= 1000) {
					pw.println("- ... truncated at 1000 functions");
					break;
				}
			}
		}
		printf("Wrote %s%n", out.getAbsolutePath());
	}
}
