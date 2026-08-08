// Forces disassembly over an address range that flow analysis did not reach.

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;

public class DisassembleRange extends GhidraScript {
	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		if (args.length != 2) {
			printerr("Usage: DisassembleRange <start> <length>");
			return;
		}
		long startValue = Long.decode(args[0]);
		long length = Long.decode(args[1]);
		Address start = toAddr(startValue);
		Address end = start.add(length - 1);
		clearListing(start, end);
		disassemble(start, end);
		printf("Disassembled %s through %s%n", start, end);
	}
}
