// Observable1.C
// Reads pals_output.root and produces two plots:
//   1. 1D histogram — first scintillator hit time for o-Ps events [ns]
//   2. 2D histogram — XZ-plane projection of the first scintillator hit position [mm]
//
// Usage (run from build/result_cache/root_file/ where pals_output.root lives):
//   root -l Observable1.C
//   root -l -q -b Observable1.C                       # batch, no window
//   root -l 'Observable1.C("my_output.root")'         # custom file

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TAxis.h"
#include "TPad.h"

void Observable1(const char* fname = "pals_output.root") {

    // ── Open file ──────────────────────────────────────────────────────────
    TFile* f = TFile::Open(fname);
    if (!f || f->IsZombie()) {
        printf("ERROR: cannot open %s\n", fname);
        return;
    }
    TTree* t = (TTree*)f->Get("PsTree");
    if (!t) {
        printf("ERROR: PsTree not found in %s\n", fname);
        return;
    }

    // ── Style ──────────────────────────────────────────────────────────────
    gStyle->SetOptStat("emri");
    gStyle->SetOptTitle(0);
    gStyle->SetPalette(kBird);
    gStyle->SetNumberContours(64);

    // ══════════════════════════════════════════════════════════════════════
    // Plot 1 — 1D: first hit time for o-Ps events
    // Positronium lifetime manifests as the time between Na22 decay and the
    // first annihilation-gamma hit in the scintillator.  Selecting
    // oPsFormed==1 && nHits>0 isolates o-Ps events that actually registered.
    // ══════════════════════════════════════════════════════════════════════
    TCanvas* c1 = new TCanvas("c1", "o-Ps first hit time", 800, 600);
    c1->SetLeftMargin(0.13);
    c1->SetBottomMargin(0.13);

    // firstHitTime is in ns absolute global time (includes Na22 decay time).
    // We bin in a wide window; the exponential tail from o-Ps lifetime is
    // superimposed on the broad Na22 decay-time distribution.
    TH1D* h1 = new TH1D("h1",
        "First scintillator hit time (o-Ps events, nHits>0);t_{hit} [ns];Events / bin",
        200, 0., 4.0e17);

    t->Draw("firstHitTime>>h1", "oPsFormed==1 && nHits>0", "goff");

    h1->SetFillColor(kAzure - 3);
    h1->SetLineColor(kBlue + 1);
    h1->SetLineWidth(2);
    h1->GetXaxis()->SetTitleSize(0.05);
    h1->GetYaxis()->SetTitleSize(0.05);
    h1->GetXaxis()->SetTitleOffset(1.1);
    h1->Draw("HIST");

    TLatex lat1;
    lat1.SetNDC();
    lat1.SetTextSize(0.04);
    lat1.DrawLatex(0.15, 0.88,
        Form("o-Ps events with scintillator hit: %lld",
             t->GetEntries("oPsFormed==1 && nHits>0")));

    c1->SaveAs("plot_oPsHitTime.png");
    printf("Saved plot_oPsHitTime.png\n");

    // ══════════════════════════════════════════════════════════════════════
    // Plot 2 — 2D: XZ-plane projection of first scintillator hit position
    // Shows where in the scintillator annihilation gammas first deposit
    // energy.  The cylindrical scintillator (axis = Y) appears as an
    // annular ring when projected onto the XZ plane.
    // ══════════════════════════════════════════════════════════════════════
    TCanvas* c2 = new TCanvas("c2", "First hit position XZ", 800, 700);
    c2->SetLeftMargin(0.12);
    c2->SetRightMargin(0.15);
    c2->SetBottomMargin(0.12);

    // Scintillator spans r = 50–70 mm in the XZ plane; set axis limits
    // slightly wider to show the full ring clearly.
    TH2D* h2 = new TH2D("h2",
        "First scintillator hit — XZ projection (nHits>0);x [mm];z [mm]",
        140, -80., 80., 140, -80., 80.);

    t->Draw("firstHitZ:firstHitX>>h2", "nHits>0", "goff");

    h2->GetXaxis()->SetTitleSize(0.05);
    h2->GetYaxis()->SetTitleSize(0.05);
    h2->GetZaxis()->SetTitle("Events");
    h2->GetZaxis()->SetTitleOffset(1.4);
    h2->Draw("COLZ");

    // Annotate inner / outer scintillator radius
    TLatex lat2;
    lat2.SetNDC();
    lat2.SetTextSize(0.035);
    lat2.DrawLatex(0.14, 0.92, "Scintillator: r_{in}=50 mm, r_{out}=70 mm");

    c2->SaveAs("plot_firstHitXZ.png");
    printf("Saved plot_firstHitXZ.png\n");

    printf("\nDone.  Plots written to:\n");
    printf("  plot_oPsHitTime.png\n");
    printf("  plot_firstHitXZ.png\n");
}
