## 1. 작업 목표

등반 곡선이 끝난 뒤 작은 잔여 이동이 개체 접촉에 막혀 원래 완료 절차를 취소하는 문제를 국소적으로 다룬다. 이미 승인된 현재 위치가 실제 종료 조건을 만족할 때만 기존 SETTLE→EGRESS로 인계하고, 실제 낙하·착지·추적·재등반과 원래 완료 집계를 보존한다. 이는 물리 질의 비용 절감 후보가 아닌 기능 계약 후보다.

## 2. 실제 수행

기존 원본·V2와 다른 default-OFF 모듈에 HoldMotionContact 직전 검증을 추가했다. 요청 일치·native curve 종료·최소 동작 시간·원본 endpoint 오차·현재 실제 exit 거리·목표 영역 LandingSpace·현재 peer/core·전체 관절 접촉·변경 없는 lift를 요구한다. 모든 조건을 통과해야 원래 Motion의 성공 블록으로 인계하며, Active를 보존해 기존 CompleteEgress가 Climbs를 집계한다. 실패는 기존 hold/backoff에 위임한다. 직접 위치 이동·lift 수정·Climbs 증가·milestone 완화는 없다. 무거운 검사는 적격 시 최대10Hz다.

합성 종료 메타데이터와 frozen upright pose로 실제 fixture/native 질의를 실행했다. 실패한 준비·컴파일·가드 시도는 모두 보존했다. 리더 참조 없음은 시험의 같은 줄 early-return 뒤 초기화 누락에서 나타났으며, 별도 문장/명시 블록 초기화 후 참조가 유지됐다. 이것을 엔진 전반의 컴파일러 결함으로 일반화하지 않는다. 낮은 목표 영역 설정도 명시 블록으로 수정했다. 첫 reference probe는 junction의 실제 경로와 실행 alias가 달라 정확한 기존 경고 가드가 차단했으며 새 probe에서 경로만 맞췄다.

중요한 음성 결과: predecessor 후보는 발이 지붕 아래0.97m인 실제 기록 입력도 종료 승인했다. 기존 발 ray의 시작이 이미 지붕 아래여서 지지를 놓치는 경로와 정지 sphere 검사만으로 내부 침투를 배제하지 못한 사례다. 새 완료 후보에만 root 위에서 시작하는 실제 두 발 ray를 추가해 목표 발판·법선·높이·발바닥 비침투를 확인했다. 기존 이동/낙하용 solver를 바꾸지 않았다. 초기4m 수평 오프셋은 미지지 입력이라는 보장이 없어 안전성 증거로 사용하지 않는다. 최종40행은 양성2·음성38의 결과와 root/lift/counter/진단 복원을 모두 assert했다. missing-leader·throttle도 포함한다.

이후 실제8명→64명 로컬 network client/server를 순차 실행했다. CUA로 장면·전경 확인 후 기존 시작 gate를 열었다. 원본 fixture·자극·완료 기준을 보존하고 native observer/영상은 추가하지 않았다. 초기 위상·날씨·전경 gate 시각이 엄밀히 같지 않으므로 성능 A/B나 과거 ID4의 정확한 재현으로 취급하지 않는다.

## 3. 측정 결과

최종 합성 가드40/40 통과: 정상 낮은·높은 지붕2행 승인, 나머지38행 거부. 직접 위치/lift/등반횟수 변경0. 합성 metadata이며 실제 authored clip 재생·개체 간 접촉 재현을 대체하지 않는다.

|시험|유효 최종 상태|low/high/down/returned 누적|낙하→착지→FOLLOW|후보 검사/완료/거부|원래 전체 gate|
|---|---|---|---|---|---|
|8명|8/8|{'low': 8, 'high': 8, 'down': 8, 'returned': 8}|9/9|0/0/0|True|
|64명|64/64|{'low': 64, 'high': 64, 'down': 64, 'returned': 64}|89/89|0/0/0|True|

검사·완료·거부는 script counter이며 CPU시간이 아니다. 상세 실행별 JSON과 선정 journal을 근거 패킷에 연결한다. 같은 실행의 재분석을 독립 반복으로 세지 않는다.

이번 실제8/64 실행의 후보 검사·완료·거부는 모두0이다. 따라서 함께 로딩한 기존 경로의 회귀 통과만 확인됐으며, 새 current-state 완료 분기의 실제 활성화 효능이나 runtime-overhead를 입증하지 않았다. 앞선 V2의ID4 접촉 취소를 고쳤다고 보고하지 않는다. 이 단일64 실행의 최종 실패0은 역사적/반복 미해결 문제 전체의 해소를 뜻하지 않는다.

8명: 70.6092초, 전이144개, phase/generation/terminal 정합 오류 0/0/0. server Mission dt p50/p95/p99(ms)=None.

64명: 292.815초, 전이1274개, phase/generation/terminal 정합 오류 0/0/0. server Mission dt p50/p95/p99(ms)={'p50': 49.0, 'p95': 73.0, 'p99': 84.0}.


위 dt는 서버 OnUpdate 간격이며 Present·CPU busy·GPU·애니메이션 표시 cadence가 아니다. 해당 지표의 p50/p95/p99·allocation/GC·반복 성능 개선은 미측정이다.20ms p95는 목표이며 달성 아님. 화면 스냅샷으로 부드러움을 판정하지 않는다.

## 4. 원인 분석

확인: 원래 Motion은 제안 이동의 접촉 거부 뒤 현재 상태의 정상 종료 가능성을 확인하기 전에 hold/backoff로 갈 수 있다. 이전64 ID4는 high/FOLLOW로 이동했어도 Active가 취소돼 authoredClimbs1에 머물렀다. 이번 후보는 current root를 독립 검증하고 원래 완료 경로를 보존한다. 합성 검증에서 지붕 아래 발을 놓치는 predecessor의 위험을 실제 입력으로 확인해 후보 범위에서 차단했다.

이번64의 이력에는 CONTACT_SUPPORT_LOST103행, ENTRY_ALIGNMENT_NO_PROGRESS4행, CLIMB_EXIT_VALIDATION4행, EGRESS_STALL1행이 남았다. 이는 실패 label이 담긴 전이 행 수이며 독립 실패 사건 수가 아니다. 최종 상태가 모두 유효하더라도 기술적 등반 종료 실패의 원인까지 해결됐다는 뜻은 아니다.89건의 낙하를 모두 개체 밀침에 의한 정상 낙하로 분류하지 않는다.

추정: 이미 유효한 현재 종료 상태의 불필요한 접촉 취소를 줄일 수 있다. 실제 효능은 후보 완료 journal이 있는 개체의 후속 EGRESS/완료 연결에서만 주장할 수 있다. 사용0의 정상 회귀는 호환성 근거일 뿐 효능 증명이 아니다. 과거 V2와 이번의 성공 숫자 차이만으로 인과·개선률을 선언하지 않는다. 전체 terminal 실패와 이력에 남은 해결된 실패를 구분하고 원시 실패 기록은 유지한다.

미확인: 동일 조건 반복 효능·추가 두 ray 비용·모든 자세/맵 안전성·8파트 화면 동기화·14클립 품질·64명20ms p95. 단일 기능 성공은 시각/성능 성공이 아니다.

## 5. 검증 수준

합성 종료 가드: 범위 한정 진단 성공.8/64 기능: 위 표의 원래 gate 결과대로 개별 판정, 독립 각1회. 반복 검증 완료·최종 수용·시각 품질·성능 목표 달성: 미완료. 종료 후 소유 프로세스 정리와 normal quit 인증을 구분하며 normalQuitVerified=false다. 전체 골은 active다.

## 6. 변경 사항

최종 격리 모듈 nf_contact_end PBO SHA256=21053F29FD2C99FA7B8DEB1AD3DD5FE792890AA76B3EBC92CF7A01CC87B03108. 자동 빌드·서명·DSCheckSignatures·packed script bytes 검증 성공. 실제 실행 전 기존7개와 후보1개 총8개 hash/서명 재검증 성공. private key는 공개/배포물에 포함하지 않았다. 기존 원본·배포PBO·V2·애니메이션 자산/그래프/native 시계·운영 서버·Steam은 변경하지 않았다. 소스/하네스/실패 자료/분석/보고서만 기존 research 구조에 추가했다.

## 7. 원시 증거

sourceRunId: network-gameplay/contact-end-current-state. candidate-guarded/explicit/feet, guards1..5, metadata/reference/pose probe, eight-contact-end3, sixtyfour-contact-end3의 build/sign/packed proofs·mission·launch·RPT/script·guard TSV·contract/milestone/terminal/gameplay·candidate-use/journal·server-frame와 분석을 해시 목록에 연결한다. 중간 실패를 삭제하거나 통과 반복으로 합산하지 않는다. 공개에는 선정 집계·자체 코드·대표 guard/journal만 제공하고 원시 전체·영상·게임자산·로컬경로·프로세스정보·키는 제외한다.

## 8. 잔여 문제

다음은 실제 candidate 완료 journal과 원래 CompleteEgress·milestone의 연결, 미해결 개체의 마지막 전이 원인을 검토하는 것이다. 후보가 사용되지 않았거나 거부됐다면 안전 조건을 낮추기 전에 해당 실제 pose/root/support 근거를 확인한다. 정상 낙하를 실패0으로 만들기 위해 제거하지 않는다. 기능 계약이 확보돼도 물리 질의 비용·실제 영상 부드러움·반복 성능은 별도 실험이다. 현재 추가 ray의 절감 효과를 주장하지 않으며 새 보간/LOD·전체 재설계를 섞지 않는다.

로컬 checkpoint SHA256: 9f1abea5d7974582040def53b65229d47a311c3633b8f62613cf22c0f4aba41a
