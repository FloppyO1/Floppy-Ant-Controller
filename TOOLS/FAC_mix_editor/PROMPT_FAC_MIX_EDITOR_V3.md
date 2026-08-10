# F.A.C. Mix Editor — work order V3

**The robot stops being two numbers and becomes a machine you lay out.** A chassis
outline you draw, wheels you place and orient, an IMU that sits somewhere on it,
and a planar kinematic model that reads all of that instead of the two-sided
unicycle of §8.

Three changes to `TOOLS/FAC_mix_editor/FAC_mix_editor.html`. **No firmware change,
no generated-code change** — the `.c`/`.h` of every preset must come out
byte-identical, and that is a gate, not an aspiration.

Read `CLAUDE.md` in this folder first. Every invariant in it still holds unless
this document overrides it explicitly, and it overrides exactly one: the robot's
kinematics (§2) and the shape of `P.sim.robot` that goes with it.
`PROMPT_FAC_MIX_EDITOR_V2.md.old` is the previous work order, shipped; §8 in it is
the model this document replaces.

---

## Ground rules

- **Still one file.** No build step, no npm, no CDN, no font, no network call. It
  must keep working from `file://` with the console clean.
- **No second implementation.** This applies to the new solver too: the yaw-rate
  readout, the renderer's slip arrows and the simulator all call **the same
  `solveTwist()`**. A closed-form `maxSpeed / track` displayed next to a machine
  the solver disagrees with is the exact bug this rule exists to prevent.
- **No float in the graph math.** The float boundary count does not change: a
  gamepad axis in `PAD.padToStage0()`, and the robot's physical state in
  `SIM.robotImuCounts()`. The new model makes the second one richer, not looser —
  everything still dies in the same `Math.round` + int16 clamp, and the self-test
  still sweeps it.
- **The sensor model is not touched.** `SIM.sensorOffsets()`, the four noise/offset
  fields and their ODR hold stay exactly as they are. They now apply to counts
  produced by a better physical model; that is all.
- **The self-test is the contract.** Export stays blocked while it has not run or
  has a failure. Every § below adds assertions.
- **Keep the section banners.** The geometry and the solver get their own,
  `SECTION: ROBOT MODEL`, immediately before `SECTION: SIM` — `SIM` is large
  enough already, and the solver is pure and belongs where it can be read alone.
- Tabs, English identifiers and UI text, comments that say *why* and cite
  `file:line` of the firmware wherever they encode firmware behaviour.

---

## Decisions already taken — do not re-litigate

These came out of the design conversation. They are settled; if the implementation
finds one of them impossible, say so and stop, do not quietly pick the other side.

1. **The physics reads the wheels.** Not a drawing that decorates the old model.
2. **There is no wheel *type*.** A wheel has a **lateral grip** in `0…1`: `1` is a
   normal tyre, `0` is an omni/mecanum roller, in between is a track that scrubs.
   The omni case and the skid-slip case are the same parameter and must not become
   two.
3. **A wheel's steering angle may come from a servo row** of the mapper, so it
   changes while the mix runs.
4. **Servos are markers.** Position, mapper row, label, live value. No arm that
   rotates, no contribution to the motion. A wheel steered by a servo (3) is a
   property of the *wheel*, not a drawn servo.
5. **The IMU has a position on the chassis**, and off the centre of rotation it
   reads the centripetal and tangential terms that position implies.
6. **The chassis is a free polygon.** The body origin is the user's `(0,0)`, the
   point their coordinates are relative to — deliberately *not* the polygon
   centroid, which would slide the machine out from under them every time they
   moved a vertex.
7. **Both editors**: numbers in the Robot tab are the truth, dragging on the view
   is a shortcut onto the same fields.
8. **The three-motor-row ceiling stays** (`fac_settings.c:103-107`). The editor may
   describe a machine the firmware cannot drive today — a four-wheel mecanum needs
   four independent motor rows and there are three — and it must **say so** where
   the user picks the link, not hide it.

---

## Phases and order of work

**§1 → §2 → §3** in one commit each, in this order, and **stop after each one**.

- **§1 Model and migration.** Data model, migration, defaults, and every existing
  call site moved onto the new fields. Nothing visible changes.
- **§2 Kinematics, the IMU at a position, and the renderer.** The machine becomes
  real, and you can see it.
- **§3 The editors.** Numeric table, then dragging on the view.

§1 and §2 are fully verifiable headless. §3 is pointer interaction and **must be
checked in a browser by the user before it is called done** — `CLAUDE.md` records
what that costs when it is skipped.

---

## §0 — Verification protocol

`CLAUDE.md` § *Verifying a change without a browser* is the recipe: build the
throwaway node harness in the scratchpad, delete it afterwards, nothing
test-shaped gets committed. Re-read it rather than reinventing it — it lists the
four gotchas of the pure-section slice, the trap where an extracted function
missing a free variable silently does nothing, and the `Proxy` canvas smoke test.

This work order adds three gates on top of it:

- **The two standing regression gates are mandatory at every §**: the generated
  `.c`/`.h` of every preset diffed against `git show HEAD:...` with the
  `FAC_MIX_EDITOR_PROJECT` trailer stripped — **they must be identical, all 16
  files, at all three §** — and the `$('#id')`-versus-markup check (`selbox` is the
  known false positive).
- **Every new assertion gets mutation-tested.** Break what it covers on purpose,
  see it go red, put it back. An assertion that compares a value against the
  default it was derived from is not an assertion.
- **The kinematics is checked against closed forms, not against itself.** The
  three below are the acceptance core of §2 and are written out there in full:
  two parallel wheels reproduce today's numbers *exactly*; a four-wheel skid turns
  by the closed form `(sR−sL) / (2b + 2·g·a²/b)`; and an IMU at radius `r` on a
  machine spinning on the spot reads `ω²r` pointing at the centre.

---

## §1 — The model: a chassis, wheels, devices, an IMU

### The new `P.sim.robot`

```js
robot: {
    maxSpeedCms: 120,             /* ground speed of a wheel at full stick */
    shape: [[10,8],[10,-8],[-10,-8],[-10,8]],   /* cm, body frame, the outline */
    wheels: [                     /* order is the display order, nothing else */
        { x: 0, y: 7, deg: 0, grip: 1, link: 'M1', steer: '', steerDeg: 30, r: 3.5 }
    ],
    devices: [ { x: 8, y: 0, link: 'S1', label: '' } ],   /* markers only */
    imu: { x: 0, y: 0 },
    /* unchanged from §8 and the sensor model */
    imuSource, gyroNoise, accelNoise, gyroBias, accelBias, viewCm, follow, trail
}
```

A wheel: `x, y` cm in the body frame; `deg` its resting steering angle, `0` =
pointing forward; `grip` the lateral grip `0…1`; `link` the mapper row that drives
it, `''` for an undriven wheel (castor, idler, a trailing wheel that only carries
weight); `steer` a mapper row whose value steers it, `''` for a fixed wheel;
`steerDeg` the travel at full deflection; `r` the drawn radius.

**Removed**: `lenCm`, `widCm`, `trackCm`, `kin`, `left`, `right`, `leftB`,
`rightB`. Everything that read them moves onto the geometry:

- `robotScale()`'s auto-fit floor used `max(lenCm, widCm)` → a `chassisBounds()`
  helper over `shape`.
- the renderer's rectangle → the polygon.
- `SIM.sideDrive()` / `SIM.sideValue()` disappear; `solveTwist()` replaces them.
- the *max yaw* readout stops being `maxSpeed / track` and calls the solver (§2).

### Migration — and the one place it deliberately changes behaviour

`migrateProject()` stays pure and keeps building onto an untouched
`blankProject()`. It synthesises the new fields from the old ones:

- `shape` ← the rectangle `lenCm × widCm`, four vertices, centred on the origin.
- `imu` ← `{x: 0, y: 0}`.
- `wheels` ← from `kin`, `left`, `right`, `leftB`, `rightB`, at
  **`y = ±trackCm/2`** — the old physics used `trackCm` and nothing else, so this
  is what keeps an old project's numbers intact — with `deg: 0`, `grip: 1`,
  `steer: ''`.
  - `kin: 'diff'` → two wheels at **`x = 0`**. The result is *bit-for-bit* the old
    model, and the self-test asserts exactly that.
  - `kin: 'skid'` → four wheels at `x = ±0.22·lenCm`, which is where the old
    renderer drew them.
- any mapper row that was linked but not a drive side → a `devices` marker.

**A migrated four-wheel skid turns more slowly than it used to, and that is a
correction, not a regression.** The old model had no notion of a wheelbase at all:
four wheels on a rigid frame *do* resist yaw, in proportion to how far apart they
are along the machine. The self-test must pin the new value to the closed form of
§2, and the Robot tab must say it once, where the grip field is. Do not "fix" this
by migrating skid wheels to `x = 0`: that would hide a real property of the
machine to protect a number that was wrong.

### Done when

- `migrateProject()` is still pure and still merges onto defaults it never writes
  into; a partial `robot` block still defaults every field it omits.
- A schema-2 file with `kin: 'diff'` produces, over a 300-step scripted run, the
  **identical** pose string and the identical IMU counts as `git show HEAD:` does
  for the same file.
- A schema-1 file (no `sim.robot` at all) still loads and drives.
- Both standing gates pass.

---

## §2 — The kinematics, the IMU at a position, the renderer

### The solver

Body frame X forward, Y left, Z up — unchanged, and it is still a statement about
how the board is *mounted* (`enum AXIS`, `LSM6DS3.h:143-147`).

The body twist is `ξ = (vx, vy, ω)`. A wheel `i` at `r = (x, y)` with steering
angle `α` has contact velocity `u = (vx − ω·y, vy + ω·x)`, and two axes of its
own: rolling `d = (cos α, sin α)`, lateral `n = (−sin α, cos α)`.

Two weighted residuals per wheel:

```
rolling   (u·d − s)²  weight 1,     only for a wheel with a drive link
                                    s = deviceValue(link)/V_MAX * maxSpeedCms
lateral   (u·n)²      weight grip,  every wheel
```

with

```
a_roll = ( cos α, sin α,  x·sin α − y·cos α )     b = s
a_lat  = (−sin α, cos α,  x·cos α + y·sin α )     b = 0
```

Minimise the sum: the normal equations `M ξ = c`, `M = Σ w·a·aᵀ` (symmetric 3×3),
`c = Σ w·a·b`. Solve by explicit Cramer on the 3×3 — no library, no iteration —
with a **Tikhonov ridge `λ` added to the diagonal** so that a machine with no
wheels, one wheel, or grip `0` everywhere still yields a finite twist instead of
dividing by a zero determinant. The ridge is load-bearing, not defensive: a
two-wheel machine at grip `0` genuinely leaves `vy` unconstrained. Document the
chosen `λ` and assert that every degenerate layout still integrates finitely — the
renderer's smoke test throws on a non-finite coordinate and that is the point.

`solveTwist()` is **pure**: layout and commands in, `(vx, vy, ω)` plus the per-wheel
residuals out. `integrateRobot()` calls it and keeps everything else it does —
still after `runGraph()`, so a pass still reads the counts of the previous
millisecond.

### The three closed forms that must hold

**Two parallel wheels at `x = 0`, `y = ±b`, grip 1.** `M` comes out diagonal,
`(2, 2, 2b²)`, `c = (sL+sR, 0, b(sR−sL))`, so

```
vx = (sL+sR)/2      vy = 0      ω = (sR−sL)/(2b) = (sR−sL)/track
```

which is §8's model exactly. **This is the first assertion to write and the last
one to let fail.**

**Four wheels at `x = ±a`, `y = ±b`, grip `g`, all forward.** The lateral rows add
`4·g·a²` to `M[2][2]` and nothing else, so

```
ω = (sR − sL) / (2b + 2·g·a²/b)
```

The lateral grip fights the yaw with a term in the **wheelbase squared** — which
is exactly why a long tracked machine turns worse than a short one, and it is the
sentence the Robot tab should carry. `g = 0` returns the two-wheel answer.

**An omni wheel is `grip = 0`** and nothing else: its lateral row vanishes, the
machine gains a `vy`, and the same solver covers mecanum with no special case.

### The slip readout

The residuals are the slip, and they are already computed:
`u·n` per wheel is how fast that contact patch is being dragged sideways (cm/s),
`u·d − s` is how much it is spinning or being dragged along its own axis. Both are
reported per wheel; the renderer draws the lateral one as a short arrow at the
contact patch, and the panel shows the largest of them. Nothing is invented here —
if a number is not a residual of the solve, it does not go on screen.

### The IMU at a position

With the twist and its derivative, the accelerometer at `r = (x, y)` reads

```
a_x = v̇x − ω·vy − α·y − ω²·x
a_y = v̇y + ω·vx + α·x − ω²·y
```

where `α = dω/dt`. At `r = 0` with `vy = 0` this is `(v̇x, ω·vx)`, which is §8's
model — so the general form *contains* the old one and must be shown to.

`v̇x`, `v̇y` and `α` are all difference quotients over a 1 ms step, i.e. spikes of
hundreds of g on any stick movement, so **all three** go through the existing
`ROBOT_ACC_TAU` first-order lag, for the reason §8 already documents. Gyro Z is
still `ω`; gyro X and Y are still zero by construction, the sketch still being
planar.

**The analytic check:** spinning on the spot at constant `ω` with the IMU at
radius `r`, once the lag has settled, the accelerometer must read `ω²r` pointing
at the centre of rotation and nothing else. Assert the vector, not the magnitude —
the sign is the half that can be wrong.

### The renderer

`drawRobotView()` keeps its one-renderer-three-homes shape and its early return on
a zero-size host. What changes inside it:

- the chassis rectangle → `shape` as a closed path;
- each wheel drawn at its position, **rotated to its live steering angle**, sized
  by `r`, filled by the drive bar it already draws, with its lateral-slip arrow;
- devices as markers with their label and the value the mapper would send;
- the IMU as a small crosshair where it sits;
- the HUD gains the largest slip; it keeps the sensor-error tag and the view line.

Smoke-test it with the `Proxy` 2D context over the awkward shapes `CLAUDE.md`
lists, plus the new ones: no wheels, one wheel, a two-vertex "polygon", a wheel
100 m off the chassis, grip `0` everywhere, a steering row that is unlinked.

### Done when

- The three closed forms above hold as assertions.
- A `kin: 'diff'` project migrated from HEAD gives an identical 300-step pose
  string and identical IMU counts.
- Degenerate layouts produce finite numbers and the renderer draws them.
- The *max yaw* readout comes from `solveTwist()` and not from a formula.
- Both standing gates pass. **The generated code is still byte-identical.**

---

## §3 — The editors

### Numeric, in the Robot tab

A table per list — wheels, devices, chassis vertices — with add and remove, one
row per item, the fields of §1, and the derived physical readout beside the ones
that have one (a steering travel in degrees, a grip as *tyre / track / roller*).
The Robot tab already owns the simulator's theory and it keeps owning it: the
grip's meaning, the wheelbase-squared sentence, the migrated-skid correction and
the three-motor ceiling live there and nowhere else.

The list rebuild rule of `CLAUDE.md` applies and is the trap to avoid: **build the
shell once and re-render only the rows.** Rebuilding the panel on every keystroke
destroys the input being typed into and takes the focus and the caret with it.

`P.sim` is not on the undo stack — `applySnap()` restores nodes, wires, mode and
instances only — so the layout editor is not undoable either. That is consistent
with every other robot field; do not make this one list the exception.

### Dragging, on the view

Wheels, chassis vertices and the IMU are draggable on the robot canvas, writing
the same fields. Constraints that come from what is already there:

- **The panel drags itself by its header** (`rbDrag()`), and the canvas pans and
  follows. A drag that starts on a handle must be decided *first*, the way the
  editor's mousedown decides a pan before a node drag — get this wrong and moving
  a wheel moves the panel.
- Hit-testing is in world coordinates through the same `robotScale()` mapping the
  renderer uses. One mapping, both directions.
- A drag that starts on nothing is not a wheel drag.
- Grid snap: reuse the existing snap preference if it is meaningful here; if it is
  not, say so in the tab rather than inventing a second snap.

### Done when

- The numeric table survives typing into it — focus and caret intact.
- Every drag writes the same field the table writes, and the table shows it live.
- **Checked in a browser by the user**, on all three panel homes (floating,
  docked right, docked in the tab), with the pane collapsed and re-opened.
- Both standing gates pass.

---

## Appendix A — deliberately not in scope

- **Dynamics.** Still no mass, no inertia, no traction limit, no motor curve: a
  step on the stick is still a step in ground speed. The lateral grip is a
  *kinematic* weight, not a friction coefficient, and the Robot tab must not let
  anyone read it as newtons. The "illustrative, a kinematic sketch" caveat stays
  on screen.
- **A fourth motor row.** Out of scope here; it is a firmware change. The editor
  describes the machine, warns, and does not pretend.
- **Wheel radius as a gearing.** `r` is drawn size only. Ground speed comes from
  `maxSpeedCms` as it always has.
- **Suspension, camber, wheels off the ground.** Every wheel is in contact.
- **Undo for the layout.** See §3.

## Appendix B — what must not move

Checked at every §, and the first three are the ones that end the work if they
break:

1. The generated `.c`/`.h` of every preset, byte for byte.
2. The two float boundaries, and the int16 clamp at the second one.
3. A `kin: 'diff'` project's pose and IMU counts, against HEAD.
4. The sensor model: gaussian noise held at the 416 Hz ODR, per-run unit-drawn
   offsets, and `robotImuCounts()` pure and deterministic.
5. `migrateProject()` pure, building onto defaults it never writes into.
6. The view as a fixed window on the ground, pixels only.
7. One copy of each explanation: block theory in the *Blocks* tab, simulator
   theory in the *Robot* tab, shortcuts in the help modal.

## Suggested commit messages

```
mix editor: §1 the robot becomes a layout - chassis, wheels, devices, IMU position
mix editor: §2 planar kinematics from the wheels, IMU off-centre, new renderer
mix editor: §3 the layout editors - numeric table and dragging on the view
```
