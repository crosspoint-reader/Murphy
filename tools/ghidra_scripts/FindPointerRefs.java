// Finds strings containing supplied keywords, then reports pointers to those strings
// and references to those pointer locations.

import java.io.File;
import java.io.PrintWriter;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Locale;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.Function;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.Reference;

public class FindPointerRefs extends GhidraScript {
	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		if (args.length < 2) {
			printerr("Usage: FindPointerRefs <out.md> <keyword> [keyword...]");
			return;
		}
		try (PrintWriter pw = new PrintWriter(new File(args[0]))) {
			pw.println("# Pointer Reference Report");
			pw.println();
			for (int i = 1; i < args.length; i++) {
				String keyword = args[i].toLowerCase(Locale.ROOT);
				pw.printf("## `%s`%n%n", args[i]);
				Data data = getFirstData();
				while (data != null && !monitor.isCancelled()) {
					if (data.hasStringValue()) {
						String value = data.getDefaultValueRepresentation();
						if (value != null && value.toLowerCase(Locale.ROOT).contains(keyword)) {
							reportString(pw, data.getAddress(), value);
						}
					}
					data = getDataAfter(data);
				}
				pw.println();
			}
		}
		printf("Wrote %s%n", new File(args[0]).getAbsolutePath());
	}

	private void reportString(PrintWriter pw, Address stringAddr, String value) throws Exception {
		pw.printf("- string `%s`: `%s`%n", stringAddr, value.replace("\n", "\\n"));
		reportRefs(pw, "direct", stringAddr, "  ");
		long offset = stringAddr.getOffset();
		byte[] needle = ByteBuffer.allocate(4).order(ByteOrder.LITTLE_ENDIAN).putInt((int) offset).array();
		for (MemoryBlock block : currentProgram.getMemory().getBlocks()) {
			if (!block.isInitialized()) {
				continue;
			}
			Address ptrAddr = block.getStart();
			while (ptrAddr != null && ptrAddr.compareTo(block.getEnd()) <= 0) {
				ptrAddr = currentProgram.getMemory().findBytes(
					ptrAddr, block.getEnd(), needle, null, true, monitor);
				if (ptrAddr == null) {
					break;
				}
				pw.printf("  - pointer `%s` in block `%s`%n", ptrAddr, block.getName());
				reportRefs(pw, "pointer", ptrAddr, "    ");
				ptrAddr = ptrAddr.add(1);
			}
		}
	}

	private void reportRefs(PrintWriter pw, String kind, Address target, String indent) {
		Reference[] refs = getReferencesTo(target);
		if (refs.length == 0) {
			pw.printf("%s- %s refs: none%n", indent, kind);
			return;
		}
		for (Reference ref : refs) {
			Address from = ref.getFromAddress();
			Function fn = getFunctionContaining(from);
			pw.printf("%s- %s ref from `%s` in `%s`%n",
				indent, kind, from, fn == null ? "<no function>" : fn.getName(true));
		}
	}
}
