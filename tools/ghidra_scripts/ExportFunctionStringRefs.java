import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Data;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import java.io.*;

public class ExportFunctionStringRefs extends GhidraScript {
  @Override public void run() throws Exception {
    String outPath = getScriptArgs().length > 0 ? getScriptArgs()[0]
                       : "/tmp/fn_string_refs.tsv";
    PrintWriter out = new PrintWriter(new FileWriter(outPath));
    out.println("string_addr\tstring\treferencing_function_addr\tfunction_name");

    // Iterate all defined strings
    var listing = currentProgram.getListing();
    var refMgr = currentProgram.getReferenceManager();
    var fnMgr = currentProgram.getFunctionManager();
    var iter = listing.getDefinedData(true);
    int n = 0;
    while (iter.hasNext() && !monitor.isCancelled()) {
      Data d = iter.next();
      if (!d.hasStringValue()) continue;
      String s = d.getDefaultValueRepresentation();
      if (s == null || s.length() < 10) continue;
      Address a = d.getAddress();
      ReferenceIterator refs = refMgr.getReferencesTo(a);
      while (refs.hasNext()) {
        Reference r = refs.next();
        Address from = r.getFromAddress();
        Function fn = fnMgr.getFunctionContaining(from);
        String fname = fn != null ? fn.getName() : "";
        String faddr = fn != null ? fn.getEntryPoint().toString() : "";
        out.println(a + "\t" + s.replace("\t"," ").replace("\n"," ") + "\t" + faddr + "\t" + fname);
        n++;
      }
    }
    out.close();
    println("Wrote " + n + " string XREFs to " + outPath);
  }
}
