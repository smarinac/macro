#include "TProof.h"
#include "TDSet.h"
#include "TFile.h"
#include "TTree.h"
#include "TSelectorDraw.h"
#include "TObject.h"


void runTSDProof(const char * vsel = "aplan", Bool_t usePDS = kFALSE)
{
	// Create TDSet
	TDSet *d = new TDSet("dset");
	d->Add("/home/smarinac/Data/dstarmb.root");

    TTree *tc = 0;
    if (!usePDS) {
        // Get light information about the tree
        TFile *f = TFile::Open("/home/smarinac/Data/dstarmb.root");
        TTree *t = (TTree *) f->Get("h42");
        t->Print();
        tc = t->CloneTree(0);
    //    tc->Print();
        tc->SetDirectory(0);
        f->Close();
    }

    // Start Proof-Lite
    TProof *p = TProof::Open("workers=1");

    if (!usePDS) {
        // Fill tree info 
        p->SetParameter("treename", "h42");
    //    tc->Print();
        p->AddInput(tc);

        // Fill input list
        p->SetParameter("varexp", vsel);
        p->SetParameter("selection", "");

        // Create selector
        TSelectorDraw *sel = new TSelectorDraw();

        // Process
        p->Process(d, sel, "goff");

        // Retrieve output
        TObject *oout = p->GetOutputList()->FindObject("htemp");

        // Draw it
        if (oout) oout->Draw();
    } else {
        p->DrawSelect(d, vsel);
    }
}
