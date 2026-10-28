# DayZ NPC AI — 외부 세션 검토 자료

Snapshot: snapshot-20261028T191735-period-fraction-20261029

8부품 표시 시간 대조와 단일 비율 후보 — 느린 평균 진행은 유지

작성: 2026-10-29T04:17:15.741778+09:00

1. 작업 목표

사용자가 보고한 반속도처럼 느린 재생과 포즈 표시 끊김을 구분한다. 기존 B8FB AI·정상 낙하/복구를 유지하고, 동일8부품의 시간 계약을 확인한 뒤 표시 비율만 바꾸는 격리 진단을 한다. 배포 수정이나 시각 수용이 목표 달성된 것으로 바꾸지 않는다.

2. 실제 수행

앞선145503 관측에서3199호출·399완전8부품 그룹의 graph 시계/분자가 정확히 일치했다. 부품마다126세대 경계가 float32 50ms 증가였고 선택6개 TRS의 이전 끝→새 시작은0m/0도, 세대 내부 endpoint 변화0이었다. 모든본/포인터 수명 전체의 증명은 아니다. 상세 읽기는 부품당p50 0.589ms여서 이번에는 시간값만 읽었다.

동일 서명 PBO·그래픽·VSync·고정 run, AI/이동OFF,1논리개체/8렌더부품,120초를 대조1회→후보1회 실행했다. 대조는24byte 복사본에 원래값을 그대로 전달했다. 후보는 이전에 완전히 관측한 세대 주기를 이용해 프레임 첫 진입에서 표시 비율을 하나 정하고8부품에 공유했다. 준비 구간 이후 복사본의 float 한 값만 변경했다. 원본 descriptor·보존 buffer·graph clock·root를 수정하지 않았다.8부품 경계까지 drain한 뒤 계측을 중단했고 script를 실행 중 hot-unload하지 않았다.

자체 모델 실제3199행 replay·공유 비율·잘못된 프레임/시계 점프 거부와 구성JS/Python 구문 검사를 수행했다. 새 baseline의 정상 종료·원시 해시·원점/독립 ACK·foreground 연결을 통과해야만 후보 진입이 가능하게 했다. 두 게임/녹화 정상 종료 뒤 영상 분석을 했다.

3. 측정 결과

|실행|숫자 변경|호출|graph/실시간 범위|HUD p50/p95/p99 ms|CPU proxy 잔차: 원본/전달 ms|후보 구간 영상쌍|
|---|---|---:|---|---|---|---:|
|runtime-20261029-040750|False|3200|0.639901–0.639909|20.140/35.267/37.061|3.058/3.058|0|
|runtime-20261029-041212|True|3200|0.640014–0.640053|21.152/34.678/36.374|11.295/2.696|297|

양쪽3200호출 모두8부품을 확보했고 오류/누락0, 보호20byte/원본24byte는 진입 시 유지됐다. 대조 수치 변경0. 후보는 준비 후3136호출 정책 적용,2328호출 실제 숫자 변경이었다. 같은값이 된 호출도 분리해 기록했다. 계측 읽기 구간 p50은 대조0.0723ms/후보0.0729ms지만 완전한 hook overhead가 아니며 NPC 성능 개선을 뜻하지 않는다.

표의 graph/실시간 약0.64는 graph-owner 시계 비율이다. 직접 clip phase나 지금 사용자가 본 화면의 정확한 배속으로 단정하지 않는다. CPU proxy는6본 source-FK 1/4키 grid 적합이며 직접 엔진 clip phase가 아니다. 영상쌍은 정확HUD로 후보 구간과 교집합을 낸 수이며 부드러움 점수가 아니다.8/64 새 기능 회귀·64 비용·GPU Present·할당/GC는 이 묶음에서 미실행이다.

CPU/source 적합 오차p95는 대조12.005mm→후보20.014mm로 커졌다. 후보 실행 내부 원래 인자/전달 인자의 CPU proxy 잔차는11.295/2.696ms였고 상관은0.435/0.953이었다. 전달값의 downstream 반응 근거는 있으나 포즈 오차 증가와 단일 대조/후보라는 회귀 위험을 함께 남긴다. 후보 표시 비율 기반 시간 증분 잔차 약2.84ms는 입력 모델의 수치이며 지각 품질 개선 수치가 아니다.

4. 원인 분석

확인: 가벼운 대조에서도8부품의 느린 graph 진행이 함께 나타났다. 후보에서도 평균 시계는 약0.64로 유지되므로 표시 비율 변경을 평균속도 해결로 보고하지 않는다. 원본/전달 인자 모델과 CPU proxy 잔차를 같은 후보 구간에서 비교해 실제 downstream 반응 근거를 남겼다. 수치는 위 표 및 검토 패킷을 따른다.

추정: 세대 주기와 표시 비율의 시간 기준 차이가 불균등 포즈에 기여할 수 있다. 후보1회는 기여 분리를 위한 시험이며 전체 끊김 원인 확정·배포 API 확보·시각 개선 반복 입증은 아니다. 반증 범위: 원래값을 유지한 대조만으로 평균속도가 정상화된다는 가정은 이번 실행과 맞지 않는다. 전체 애니메이션/전환/64개체로 일반화하지 않는다.

5. 검증 수준

진단 성공:8부품 시간 계약, 정상 종료 대조/후보의 원시 수집·CPU/source proxy·실제 녹화 연결. 모델 replay/입력 거부는 도구 검사 통과다. 반복 검증 완료는 원본/후보의 효과에 아직 해당하지 않는다. 시각 품질·평균속도 수정·8→64 회귀·20ms p95·최종 수용은 미수용이다.

6. 변경 사항

trace_horde_native_calls.py와watch_horde_frame_order_calls.ps1에 명시적 periodBaseline/periodCandidate plan을 추가했다. horde_shared_period_model.js·horde_period_fraction_probe.js·전용 분석기/입장 gate가 진단용 변경이다. 기존 기본plan과 게임 실행계층은 유지했다. baseline/candidate의 실제 archived driver/JS/config를 근거로 사용한다. 서명 PBO B8FB/453D/0C6E는 변경·재빌드하지 않았고 두 실행마다3개 DSCheck 서명 검증 성공. 개인키/자산은 공개하지 않는다.

7. 원시 증거

runtime-20261029-040750과041212의 정상 종료 metadata·실행 source/config·3200행씩 native·HUD/CPU·독립 시계·45초 영상·정확HUD/포즈 연결·분석 JSON을 보존한다. 앞선145503 및 replay도 원시 해시로 연결한다. 공개 패킷은 집계와 첫4개 CPU/source proxy 연결쌍, 자체 순수 모델만 선정한다. 전체 trace나 영상 공개가 아니다. 자료저장 전/후 해시·metadata·JSON을 기존 게시기로 검사하며 원본과 과거 보고서를 덮어쓰지 않는다.

8. 잔여 문제

다음은 후보 ON/OFF가 실제영상의 포즈 증분에 미치는 영향을 같은 구간에서 정량 비교하고, 역순 대조/후보를 반복하는 것이다. 평균 graph 속도 원인은 별도로 실제 dt 전달/소비를 추적해야 한다. 후보는 native 진단이라 배포 가능한 수정이 아니다. 지지/접촉·공격/착지 타이밍이나 정상 낙하를 약화하지 않는다.

기존64 최종64/64와 HIGH_HOLD 미도달13/14/43, EGRESS_STALL1·climb-end0을 이 새 애니메이션 시험으로 해결됐다고 갱신하지 않는다. 사용자 시험의 SetTemperature 예외도 별개 미해결이다.64 비용은 별도 통제 ETW/함수 계측·반복 비교가 필요하고20ms p95는 계속 목표다.

원본 보고서 ID: 2026-10-29_041715_741778+0900_npc-period-fraction.md
원본 SHA-256: 765eb60c67b5e71367c1dbeae1dea2f0749a68d12d6e5a4ae61b9243dacf8f00
이 텍스트는 복사용 파생본입니다. 수치·검증 한계는 원본과 같으며 상세 파일 해시 목록은 원본에 보존되어 있습니다.

선정 집계/첫4개 쌍
[
  {
    "summary": {
      "runId": "runtime-20261029-040750",
      "buildId": "B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185",
      "scenario": "anim1",
      "count": 1,
      "durationSeconds": 120,
      "highHoldWindow": null,
      "omitGameplayMetrics": true,
      "foregroundMissionFrameMs": {
        "p50": 20.13995,
        "p95": 35.26696,
        "p99": 37.061455
      },
      "calls": 3200,
      "completeReadyFrames": 394,
      "numericOverride": false,
      "graphClockVsWallRange": [
        0.6399012123221083,
        0.6399086633356003
      ],
      "entryReadBracketP50Ms": 0.07229999999999999,
      "clockAcks": 227,
      "exactVideoHudFrames": 360,
      "cpuPairs": 393,
      "sourceFitP95Meters": 0.012004616888442158,
      "originalVsCpuProxyResidualRmsMs": 3.0577774321032107,
      "submittedVsCpuProxyResidualRmsMs": 3.0577774321032107,
      "exactVisiblePairsInsideNumericOverride": 0,
      "nativeWindowMissionSeconds": [
        24.652,
        32.53
      ],
      "originalArgumentTimingResidualMsRange": [
        10.834995538006343,
        10.98136984788032
      ],
      "submittedArgumentTimingResidualMsRange": [
        10.834995538006343,
        10.98136984788032
      ],
      "measurementConditions": {
        "logicalAgents": 1,
        "renderParts": 8,
        "clip": "run",
        "ai": false,
        "movement": false,
        "repetitions": 1,
        "descriptorPointerReplacement": true,
        "descriptorValuesChanged": false,
        "durationSeconds": 120,
        "hookSeconds": 8,
        "performanceAcceptance": false,
        "frameSync": "on",
        "admissionReapproach": false,
        "method": "foreground mission callbacks; native outer-entry / source-FK proxy / exact video HUD, not GPU Present"
      }
    },
    "consumerLimitations": [
      "Six-point source-FK phase proxy, not direct clip phase or perceptual quality.",
      "Visible pair intersection locates candidate coverage; it is not a quality score.",
      "One baseline and one candidate; repeat/crossover not completed."
    ],
    "selection": "First4 CPU/source proxy pairs, not entire trace; first complete8-part group timing policy is in retained model.",
    "representativePairs": [
      {
        "firstFrame": 1193,
        "lastFrame": 1194,
        "missionSeconds": 24.688,
        "wallDeltaMs": 4.9281999999999995,
        "originalDeltaMs": 6.000000052154064,
        "submittedDeltaMs": 6.000000052154064,
        "sourceProxyDeltaMs": 8.333333333333334
      },
      {
        "firstFrame": 1194,
        "lastFrame": 1195,
        "missionSeconds": 24.696,
        "wallDeltaMs": 36.6142,
        "originalDeltaMs": 35.99999938160181,
        "submittedDeltaMs": 35.99999938160181,
        "sourceProxyDeltaMs": 33.333333333333336
      },
      {
        "firstFrame": 1195,
        "lastFrame": 1196,
        "missionSeconds": 24.725,
        "wallDeltaMs": 8.706,
        "originalDeltaMs": 7.999237626791,
        "submittedDeltaMs": 7.999237626791,
        "sourceProxyDeltaMs": 8.333333333333334
      },
      {
        "firstFrame": 1196,
        "lastFrame": 1197,
        "missionSeconds": 24.734,
        "wallDeltaMs": 28.962600000000002,
        "originalDeltaMs": 28.00000086426735,
        "submittedDeltaMs": 28.00000086426735,
        "sourceProxyDeltaMs": 29.166666666666668
      }
    ],
    "counts": {
      "interpolate": 3200,
      "numericApplied": 0,
      "numericChanged": 0
    },
    "proofSha256": "89eef5c7acdead2576f2d88c0201543737a88684c0906ec2d2067407281609ea"
  },
  {
    "summary": {
      "runId": "runtime-20261029-041212",
      "buildId": "B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185",
      "scenario": "anim1",
      "count": 1,
      "durationSeconds": 120,
      "highHoldWindow": null,
      "omitGameplayMetrics": true,
      "foregroundMissionFrameMs": {
        "p50": 21.15195,
        "p95": 34.67803,
        "p99": 36.373760999999995
      },
      "calls": 3200,
      "completeReadyFrames": 392,
      "numericOverride": true,
      "graphClockVsWallRange": [
        0.6400141282357875,
        0.6400527878634629
      ],
      "entryReadBracketP50Ms": 0.07289999999999999,
      "clockAcks": 245,
      "exactVideoHudFrames": 360,
      "cpuPairs": 391,
      "sourceFitP95Meters": 0.020013785144626206,
      "originalVsCpuProxyResidualRmsMs": 11.294650193576176,
      "submittedVsCpuProxyResidualRmsMs": 2.6955345557274843,
      "exactVisiblePairsInsideNumericOverride": 297,
      "nativeWindowMissionSeconds": [
        24.173,
        32.006
      ],
      "originalArgumentTimingResidualMsRange": [
        10.861862747853559,
        11.010107931195233
      ],
      "submittedArgumentTimingResidualMsRange": [
        2.829187597359413,
        2.845745712117348
      ],
      "measurementConditions": {
        "logicalAgents": 1,
        "renderParts": 8,
        "clip": "run",
        "ai": false,
        "movement": false,
        "repetitions": 1,
        "descriptorPointerReplacement": true,
        "descriptorValuesChanged": true,
        "durationSeconds": 120,
        "hookSeconds": 8,
        "performanceAcceptance": false,
        "frameSync": "on",
        "admissionReapproach": false,
        "method": "foreground mission callbacks; native outer-entry / source-FK proxy / exact video HUD, not GPU Present"
      }
    },
    "consumerLimitations": [
      "Six-point source-FK phase proxy, not direct clip phase or perceptual quality.",
      "Visible pair intersection locates candidate coverage; it is not a quality score.",
      "One baseline and one candidate; repeat/crossover not completed."
    ],
    "selection": "First4 CPU/source proxy pairs, not entire trace; first complete8-part group timing policy is in retained model.",
    "representativePairs": [
      {
        "firstFrame": 1167,
        "lastFrame": 1168,
        "missionSeconds": 24.205,
        "wallDeltaMs": 8.0343,
        "originalDeltaMs": 8.999999612569809,
        "submittedDeltaMs": 5.151071585714817,
        "sourceProxyDeltaMs": 4.166666666666667
      },
      {
        "firstFrame": 1168,
        "lastFrame": 1169,
        "missionSeconds": 24.211,
        "wallDeltaMs": 32.3709,
        "originalDeltaMs": 32.00000151991844,
        "submittedDeltaMs": 20.162255503237247,
        "sourceProxyDeltaMs": 20.833333333333332
      },
      {
        "firstFrame": 1169,
        "lastFrame": 1170,
        "missionSeconds": 24.247,
        "wallDeltaMs": 5.7543999999999995,
        "originalDeltaMs": 6.000000983476639,
        "submittedDeltaMs": 3.591693937778473,
        "sourceProxyDeltaMs": 4.166666666666667
      },
      {
        "firstFrame": 1170,
        "lastFrame": 1171,
        "missionSeconds": 24.256,
        "wallDeltaMs": 36.889199999999995,
        "originalDeltaMs": 2.9992349445819855,
        "submittedDeltaMs": 21.094216033816338,
        "sourceProxyDeltaMs": 20.833333333333332
      }
    ],
    "counts": {
      "interpolate": 3200,
      "numericApplied": 3136,
      "numericChanged": 2328
    },
    "proofSha256": "de2927a61b1e6ccfad6fa3287cdd53a93e348c1885b7ed3482496bb897299309"
  }
]

자체 순수 시간 모델 — 엔진 접근/배포 정책 없음
// Pure shared-frame timing model. No engine access or deployment policy.
function SharedPeriodModel(frequency) {
 this.frequency=frequency;this.key=null;this.seen=new Set();this.parts=new Set();this.fullFrames=0;this.frameIndex=-1;
 this.graphClock=null;this.generationStart=null;this.trustedBoundary=false;this.previousPeriod=null;this.frameFraction=null;
 this.frameTick=null;this.completedFrames=0;
}
SharedPeriodModel.prototype.step=function(part,graphClock,numerator,duration,tick){
 if(![graphClock,numerator,duration,tick,this.frequency].every(Number.isFinite)||duration<=0||this.frequency<=0||numerator<0)throw Error('Model bounds');
 if(!Number.isInteger(part)||part<0||part>7)throw Error('Model part');
 this.parts.add(part);
 const key=graphClock+'|'+numerator;
 if(key!==this.key||this.seen.has(part)){
  if(this.key!==null){
   if(this.seen.size===8){this.fullFrames++;this.completedFrames++;}
   else if(this.fullFrames>0)throw Error('Incomplete frame after complete cohort');
  }
  this.key=key;this.seen.clear();this.frameTick=tick;this.frameIndex++;
  if(this.graphClock===null){this.graphClock=graphClock;this.generationStart=tick;}
  else if(graphClock!==this.graphClock){
   const expected=Math.fround(this.graphClock+duration);
   if(expected!==graphClock)throw Error('Unexpected graph-clock step');
   if(this.trustedBoundary){
    const period=(tick-this.generationStart)/this.frequency;
    if(period<0.02||period>0.25)throw Error('Observed period outside diagnostic bounds');
    this.previousPeriod=period;
   }
   this.graphClock=graphClock;this.generationStart=tick;this.trustedBoundary=true;
  }
  this.frameFraction=this.previousPeriod===null?null:Math.max(0,Math.min(1,(tick-this.generationStart)/this.frequency/this.previousPeriod));
 }
 this.seen.add(part);
 const ready=this.parts.size===8&&this.fullFrames>=2&&this.frameFraction!==null;
 return {frameIndex:this.frameIndex,ready,proposedFraction:ready?this.frameFraction:null,
         previousPeriodSeconds:this.previousPeriod,completeCurrentFrame:this.seen.size===8};
};

이전 애니메이션 진단: [선정 근거](downloads/animation-review.txt). 이번 진단은 시각·성능 해결을 뜻하지 않는다.
