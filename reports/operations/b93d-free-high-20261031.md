# B93D 보정 없는8→64시험: 패키지 계보와 플레이어 고정 조건 분리

작성: 2026-10-31T12:40:39.209355+09:00

## 1. 작업 목표

보존 B8FB의 고지대 누락12명과 후속 B93D의 과거 통과 결과 사이에 있는 시험 조건 차이를 분리한다. 현재 공용 D61D를 덮어쓰지 않고 실제 패키지 계보와 런타임 플래그를 확인한다.

## 2. 실제 수행

실제 PBO member를 비교했다. B8FB→공용D61D는4script차이이며, 공용D61D→B93D는6script차이다. 파일 포함과 런타임 활성화는 별개로 분류했다. 과거B93D anchorON64반복 실행20261031-075221의 launcher hash·flags를 대조했다. 기존 실행기를 별도 폴더로 파생해 high-player-anchor.flag 생성만 선택적으로 생략하고 이를 launch에 명시했다. NPC코드·PBO·미션은 변경하지 않았다. 기존 서명3개를 매 실행 검증했다.

새8/300→64/300실행은 모두 anchorOFF, 같은B93D·compiledmission·옵션을 사용했다. 과거 실행과 같은 noVisualCapture/noDriveAttribution을 유지했다. 컴퓨터유즈로 실제 장면을 관측했으나 시각 부드러움 판정이나 영상을 새로 생성하지 않았다. 정상 종료 뒤 기존5분석기와 player-stimulus 대조를 수행했다. 높은 곳에서 플레이어가 떨어졌다면 해당 고지대 비교를 무효로 판정하도록 미리 규정했다.

## 3. 측정 결과

| 실행 | 명 | 최종ARRIVED | 고지대누락ID | 낙하복귀 | 등반종료실패 | HIGH_HOLD플레이어영역 | 해당시험기능기준 |
|---|---:|---:|---|---:|---:|---|---|
|runtime-20261031-122659|8|8|[]|10|0|{'high': 519}|True|
|runtime-20261031-123319|64|64|[]|81|0|{'high': 459}|True|

- eight: p50/p95/p99 {'samples': 12900, 'p50': 20.0, 'p95': 33.0, 'p99': 34.0}; unresolvedTerminal []; admission violations []; egress violations []; physical transitions 116 same-call matched True; stalls [].
- sixty-four: p50/p95/p99 {'samples': 2375, 'p50': 57.0, 'p95': 111.0, 'p99': 125.0}; unresolvedTerminal []; admission violations []; egress violations []; physical transitions 966 same-call matched True; stalls [].

표본 HIGH_HOLD 위치변화는 각stimulus JSON에 보존했다. 고지대·최종·계약을 독립 판정했다.20ms p95는 미달성 목표이며 위 mission frame자료는 성능 최적화 검증이 아니다.

## 4. 원인 분석

확인된 사실: 과거B93D시험에는 플레이어가 중심에서0.1m 이상 이동하면 Floor검사 후 위치를 보정하는 high anchor가 있었다. B8FB에는 없었다. B8FB64의 HIGH_HOLD480표본은 모두high였지만21위치로 변했다. 따라서 과거두패키지의 통과차이를 코드만의 효과로 귀속할 수 없다. 새OFF시험은 B93D자체의 보정없는 기능 범위를 보여주며, B8FB대비 여러 패치 전체의 효과를 한수정으로 분해하지 않는다.

가설: 목표의 군집 접촉 변위가 정렬·진입·복구 과정에 영향을 줄 수 있다. anchorOFF시험이 통과하더라도 이 가설을 일반적으로 반증하거나, 실패시 anchor하나를 물리근본원인으로 확정하지 않는다. 특정ID의 상태·목표·접촉 전이를 후속 분석해야 한다. 정상 낙하·착지·복귀는 보존한다.

미확인: 최종GPU표시 소비경로·배포가능 residual clock제어·실제 애니메이션 부드러움·로깅 없는64비용귀속·할당/GC. 이전 CPU/화면 시간진단은 이 기능실험과 별개다.

사용자 추가 기준:64명이 좁은컨테이너에 동시에설필요없다.다른개체에밀려떨어져도유효착지·재추적·재등반으로이어지면정상으로인정한다.기존고지대기준도동시점유가아닌구간내개별도달이었다.이번원판정은보존하고후속기준에재등반연결증거를추가한다.

실제코드:ManagedPeerContacts가peerhard-core진입거부를우회하지만개체쌍밀침·플레이어검사·worldSphereCast·SolveContacts는남아있다.단등반중밀침은벽접선방향/최대offset0.35m/미래착지footprint보호로제한한다.따라서충돌을전부끈것은아니지만자유로운공중밀침·낙하를완성했다고볼수도없다.FOLLOW중지지상실은pressurefall을시작한다.선정함수원문과실측누적counter를보존했다:

- eight: {'pressureFalls': 2.0, 'pressureSteps': 14245.0, 'pressureBlocks': 15.0}
- sixty-four: {'pressureFalls': 19.0, 'pressureSteps': 128001.0, 'pressureBlocks': 3463.0}

누적counter는각개체마지막표본을합한것이다.전체낙하81회가모두개체밀침원인이라는뜻은아니다.최초contact-summary의pressureRejectReason필드명은실제TSV의pressureReject와달라그histogram은사용하지않는다.v2자료에실제필드를연결했다.

## 5. 검증 수준

패키지 계보·조건차이: 진단 성공. 새실행: 표의 개별판정이며 각count1회. 반복검증완료·최종수용은 아니다. 기능·시각·성능을 합쳐 성공으로 보고하지 않는다. 원래B8FB8/180실패와64고지대52/64를 그대로 보존한다.

## 6. 변경 사항

기존signed B93D b93d2e713fe7aeb41570c2b21839ff1ada148be0eac090747220b6a4f78ad0d1 사용. 공용D61D hash유지. 새PBO·재서명 없음, 기존서명3개 각실행 검증성공. 새로 만든 것은 실행기파생·플래그기록·분석·보고뿐이다. 운영서버·개인키·원본기록은 변경하지 않았다.

## 7. 원시 증거

이번폴더의 packaged-diff/candidate-packaged-lineage/launcher.diff/free-high-plan, 각launch출력/results/stimulus/분석기log, 두runtime의 원시TSV·전이CSV·미션snapshot·compileproof를 보존한다. 저장전기존hash·JSON·안정성·정상종료를 기존게시기로 검증한다. 공개본은 선정집계이며 로컬경로·프로세스·raw전체·키를 제외한다.

## 8. 잔여 문제

새실행에서 남은고지대·최종·계약실패는 각entity/recoverygeneration/실제contact와연결해 최소후보를 시험한다. 보정없는동일조건을 반복해 재현성을 확보한 뒤 B안반영여부를 결정한다. 현재공용파일을 후보로교체하지 않았다. 애니메이션의확인된 overshoot손실과 배포가능제어경로,64성능비용은 별도통제실험으로 계속한다.

사용자피드백후속:동시점유목표를추가하지않는다.유효접근/정상변위·낙하/착지/추적유지/재등반가능성을개체세대별연결해검증한다.등반중접촉제한이부자연스러운경우에한해별도후보에서제약한개씩시험하며벽·실제지지검사를통째로삭제하지않는다.피드백은기록됐으며해당접촉재설계구현·검증은아직미완료다.

선정 공개 본문. 로컬 보고서 SHA-256: `691214d1df503fc1003f1cbb7cc6ce0a87192039953c8eb44be4a05d27b64211`. 원시 전체가 아니다.
