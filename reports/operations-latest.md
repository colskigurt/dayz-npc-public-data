## 1. 작업 목표

앞선8/64 회귀에서 후보 사용0이었던 한계를 줄이기 위해, 실제 authored 등반의 다음 이동만 플레이어 접촉에 거부되는 순간을 재현하고 새 current-state 완료가 기존 SETTLE/EGRESS/CompleteEgress로 연결되는지 확인한다. 원래 낙하·지지·접촉 검사는 유지한다. 자연 군집 재현·반복 성능·영상 품질 시험이 아니다.

## 2. 실제 수행

기존8파트 network NPC1개체와 실제 시험용 플레이어를 사용했다. mission의 traversal 하위 클래스가 원래 Accept를 호출하기 직전 nativeCurveEnded·elapsed·endpointError와 현재 지원된 위치를 읽고, 플레이어만 배치한다. 실제 CoreClear(current,current)=true, CoreClear(current,next)=false·PLAYER_CORE인 경우에만 arm한다. 그 뒤 원래 Accept가 거부하고 원래 Motion이 HoldMotionContact를 호출한다. NPC root/lift/시간/요청/curve 종료/counter를 주입하지 않았다. 최종 후보 PBO21053F29는 변경하지 않았다.

첫 residual 방향 배치 OFF/ON은 native 실행에서 후보를 활성화하는 목적이다. OFF도0.15초 뒤 스스로 완료해 bounded 취소를 재현하지 못했다. 이어 벽 정면 방향과 실제 exit의 접선 좌표를 기준으로 플레이어를 고정하는 axis OFF/ON을 수행했다. 각 쌍의 mission bytes·8PBO·binary는 동일하며 candidate flag만 다르다. 초기 native 위상·날씨·foreground gate 시각·실제 개체 궤적은 같게 고정하지 않았으므로 시간 차이를 개선률로 사용하지 않는다.

첫 쌍은 test subclass로 traversal을 교체한 뒤 기존 mission의 명시적 EGRESS-only 활성화가 옮겨지지 않았다. 두 조건 모두 동일한 cold/default 설정이므로 활성화 진단으로만 사용한다. axis 쌍에서는 기존 ECEnabled/first-step 등6개 정책을 다시 명시해 복원했다. 이 차이를 candidate 효과로 합산하지 않는다. CLI의 newPbo=true와 초기 READY의8/64 문구는 복사한 템플릿 표식이다. 실제 Count1·파트8·새PBO0이며 전용 결과 TSV가 판정 근거다. 원래8명 전체 milestone terminal은 의도적으로 incomplete/functional=false로 닫았고, 이를 실제8명 실패 또는 통과로 바꾸지 않는다.

CUA로 실제 게임 장면·전경을 확인한 뒤 시작 gate를 열었다. 새 native hook·영상·대용량 dump·도구 설치는 하지 않았다. 모든 실험 종료와 소유 프로세스 정리를 확인한 뒤 기록했다.

## 3. 측정 결과

|조건|실제 arm|후보 검사/완료/거부|누적 contact hold|최종 상태 / 완료 횟수|최종 failure|
|---|---|---|---|---|---|
|contact-end-runtime-off|true|0/0/0|0.15s|FOLLOW / climbs2|없음|
|contact-end-runtime-on|true|2/1/1|0.1s|FOLLOW / climbs2|없음|
|contact-end-axis-off|true|0/0/0|2.1s|FOLLOW / climbs1|SUPPORTED_CONTACT_REPLAN|
|contact-end-axis-on|true|16/0/16|2.05s|FOLLOW / climbs1|SUPPORTED_CONTACT_REPLAN|
|contact-end-axis-lift-probe|true|17/0/17|2.1s|FOLLOW / climbs1|SUPPORTED_CONTACT_REPLAN|

actual activation verified=True; OFF bounded cancellation reproduced=True. 각 조건 독립1회다. 두 다른 배치를 같은 조건 반복2회라고 세지 않는다. native trigger는 전용 결과의 실제 request·elapsed·duration·endpoint·current/proposed clear·위치로 연결한다. 후보 journal의 lift=checkedLift도 확인했다. 최초 거부의 세부 진단이0.25초 샘플 사이에 갱신됐다면 counter만으로 원인을 특정하지 않는다.

중요한 한계: axis 단일 대비 성공=False. axis OFF와ON 모두 SUPPORTED_CONTACT_REPLAN으로 취소됐고 완료 횟수1에 머물렀다. ON의16검사/16거부와 반복 표본의 UNCHANGED_LIFT를 확인했다. 추가 계측 native 실행의 17행에서 deltaLift=0.0038147~0.0038147m, contactCount/jointCount 모두4 일치=True. solver는 현재 위치의 포즈 접촉을 수용했지만 checkedLift가 이전 lift보다 약3.815mm 커서 후보의0.1mm 불변 조건이 거부했다. 그 조건이 지속 정체를 해결하지 못하는 직접적인 가드임을 좁혔으며, 아직 완화하거나 새 높이를 적용하지 않았다. 추가 계측 실행을 원래 axis 조건의 독립 반복 성공으로 합산하지 않는다.

새8/64 회귀는 수행하지 않았다. 앞선 동일 후보의8/8·64/64·89/89복귀는 보존하지만 실제 후보 사용0이므로 활성화된64명 검증을 대신하지 않는다. Present/CPU busy/GPU p50/p95/p99·animation visible cadence·할당/GC·후보 추가 질의의 순수 비용은 미측정.20ms p95 목표 미달성이다.

## 4. 원인 분석

확인: 현재 ManagedPeerContacts 설정의 CoreClear는 감염자 간 중심 배제를 군집 처리로 분리하고 PlayersClear를 사용한다. 따라서 이 경로를 감염자끼리의 접촉 대기로 단정하지 않는다. 이번 실제 PLAYER_CORE 거부는 읽기 전용 수식 추정이 아닌 원래 함수의 실행 결과다. 현재 위치가 안전해도 잔여 제안 이동이 플레이어0.62m 제외 범위에 들어가면 거부될 수 있다.

후보는 이미 안전한 현재 상태를 검증한 뒤 원래 정상 완료 블록에 인계한다. 구체적인 OFF 취소와 ON 완료 대비는 아래 원시 전이/journal로만 판단한다. 다른 맵·모든 자세·과거ID4와의 완전 동일성은 입증하지 않았다. 짧은 대기 후 기존 경로도 완료한 첫 쌍을 실패로 재분류하지 않는다. 첫 쌍의 시각·종료 시간 차이를 성능 향상으로 계산하지 않는다.

미확인: 현재 위치 자체가 플레이어에 겹친 경우의 실제 음성 대조·동일 axis 독립 반복·활성화된8/64 군집·반복 비용·전체14클립 시각 품질. 실제 한 번 발동했다는 사실은 일반 수용과 다르다.

가설/후보(미구현): 기존 Accept는 SolveContacts가 검증한 lift와 contacts를 함께 적용한다. 새 current-state 완료에도 원래 solver의 작은 상향 보정만 한정해 수용할 여지가 있다. 예를 들어 별도 실험에서 상향1cm 이내·원래 lift cap·4관절 swept 검사·목표 발판/양발 비침투·현재 player/core·native 종료를 모두 유지하고, 검증된 checkedLift와 contacts만 함께 반영한 뒤 기존 SETTLE/EGRESS로 넘기는 방식이다.1cm는 제안한 실험 상한이며 안전성 입증값이 아니다. 위험은 부적절한 높이 점프·기존 pose/foot 계약 변경이며, 작은/큰 delta 양성·음성 가드와 실제 axis 대조 후8→64 검증이 필요하다. root·시간·완료 횟수 강제 변경이나 지지 true 처리는 금지한다.

## 5. 검증 수준

실제 authored 동작→PLAYER_CORE 거부→새 검증→기존 완료 연결: 위 activation 결과 범위에서 진단 성공. bounded cancellation 대비는 위 OFF 결과 범위의 단일 대조다. 자연64의 이전 실패 전체 해결·반복 검증 완료·최종 수용·시각 품질·성능 성공은 미완료다. normalQuitVerified=false이며 wrapper exit와 정상 게임 종료 인증을 구분한다. 전체 골 active.

## 6. 변경 사항

새 PBO 없음. 기존 최종 후보21053F29 및7개 모듈은 각 실행 전hash/서명검증 성공했다. 기존 후보 빌드·서명 성공 이력을 유지하며 이번에 새 서명을 했다고 보고하지 않는다. 새 mission 하네스·실행 설정·전용 분석/보고서만 기존 research 아래 추가했다. 운영서버·Steam·원본 자산·graph/native 시계·기존PBO는 변경하지 않았다. private key는 공개/배포물에 없다.

## 7. 원시 증거

sourceRunId: network-gameplay/contact-end-real-player-core. contact-end-runtime-off/on, contact-end-axis-off/on의 plan·동일 mission hash·8PBO signature/hash·서버/클라이언트 종료·script/RPT·전용23열 result·원래27열 state contract·candidate14열 journal·41열 gameplay·8열 사용 기록과 분석을 해시 목록에 연결한다. 공개 패킷은 선정 결과·전이·자체 fixture 코드만 제공하고 원시 전체·게임 자산·영상·로컬 경로·프로세스·키는 제외한다.

## 8. 잔여 문제

다음은 약3.815mm의 검증된 lift 차이를 원래 Accept의 적용 계약과 대조하고, 위 제한적 높이 수용 후보를 격리 가드부터 검증하는 것이다. 현재 core가 막힌 음성·큰 lift 변화 거부·반대 순서 actual axis 대조를 보존한 뒤8/64로 연결한다. 아직 지속 정체가 해결됐다고 보고하지 않는다. 물리 질의 비용과 애니메이션 시각 품질은 별도 우선 과제이며 이 기능 실험을 비용 최적화 성과로 취급하지 않는다. 기존 scratch/SupportFast 근거를 재사용하고 근거 없는 cache/새 보간을 섞지 않는다.

로컬 checkpoint SHA256: a2b31fd38a49172dc20083e2ff82331e3d378765bacffb27aced8d23f6cf68f1
