// Finds strings containing supplied keywords and reports references/functions.

import java.io.File;
import java.io.PrintWriter;
import java.util.Locale;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;

public class FindStringRefs extends GhidraScript {
	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		if (args.length < 2) {
			printerr("Usage: FindStringRefs <out.md> <keyword> [keyword...]");
			return;
		}
		File out = new File(args[0]);
		try (PrintWriter pw = new PrintWriter(out)) {
			pw.println("# String Reference Report");
			pw.println();
			for (int i = 1; i < args.length; i++) {
				String keyword = args[i].toLowerCase(Locale.ROOT);
				pw.printf("## `%s`%n%n", args[i]);
				int matches = 0;
				Data data = getFirstData();
				while (data != null && !monitor.isCancelled()) {
					if (data.hasStringValue()) {
						String value = data.getDefaultValueRepresentation();
						if (value != null && value.toLowerCase(Locale.ROOT).contains(keyword)) {
							matches++;
							Address addr = data.getAddress();
							pw.printf("- `%s`: `%s`%n", addr, value.replace("\n", "\\n"));
							Reference[] refs = getReferencesTo(addr);
							if (refs.length == 0) {
								pw.println("  - refs: none");
							}
							for (Reference ref : refs) {
								Address from = ref.getFromAddress();
								Function fn = getFunctionContaining(from);
								pw.printf("  - ref from `%s` in `%s`%n", from, fn == null ? "<no function>" : fn.getName(true));
							}
						}
					}
					data = getDataAfter(data);
				}
				pw.printf("%nMatches: %d%n%n", matches);
			}
		}
		printf("Wrote %s%n", out.getAbsolutePath());
	}
}
