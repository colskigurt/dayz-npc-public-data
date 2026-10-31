# 보간 상태·CPU·영상144쌍 부분 연결: reset 경계104개는 미확인

작성: 2026-10-31T10:31:57.253419+09:00

## 1. 작업 목표

큰 포즈 step이 발생한 영상 프레임을 실제 native 보간 상태·클립 입력과 연결한다. 기존 느린 clip 시계 진단 이후의 실행 계층 문제를 좁히되 캐시와 최종 표시를 혼동하지 않는다. 기존 NPC 구현·정상 낙하/복구·운영 서버는 보존한다.

## 2. 실제 수행

기존 B8FB archive,1논리개체/8파트,고정 run2,AI/루트 이동 OFF,queryPose ON,VSync ON,120초 실행. 기존3개 root/dispatcher/clip entry 관찰에 보간·transform 적용 뒤4356b0 진입1개를 추가했다. return b72578·animal vtable·renderer 소유·73본·scheduler mode0를 검증하고 query188의6관절 캐시 좌표와 graphClock/accumulator/duration/최근 clip sample을8초 기록했다. 인자/시간/포즈 override와 return hook은 없다. 훅은 비활성화 후 정상 종료까지 유지했다.

기존 OBS WGC로 DayZ 창만45초·1600×900·60fps·CRF0 녹화,음성/바탕화면 제외. 녹화와게임 모두 정상 종료했다. contact sheet4시점을 직접 검토했다. 영상5/8/11초부터 각3초를 기존 glyph/HUD·CPU/윤곽 분석으로 연결했다. Steam 알림이40초 화면 우하단에 보이지만 선정 actor/HUD ROI나 공개 자료에는 포함하지 않는다.

독립6관절 교정의 raw 파일2개가 최소 작업본에 없어 Steam 복구본에서20,405,871bytes와1,025,718bytes만 복원했다. 기존 복원 도구가 archive/member 해시를 검증했고 기존 파일을 덮어쓰지 않았다. 교정은 background448frame의6관절 매핑을 재검증한 것으로 foreground 표시 시간의 교정은 아니다.

시계 bridge는 기존 nonce/ACK 프로토콜을 재사용했지만 새 helper의 마지막 검사가 TickCount를 절대 QPC와 동일시해 실패했다. 원시38ACK를 보존하고 기존 도구의 offset interval 방식을 별도 분석기로 적용했다. 원점 차이가 있는 두 시계에서 unwrap·ACK 전후 QPC 구간의 교집합을 검증했다. helper 원본/오류는 보존했고 성공으로 바꾸지 않았다. 게임을 재시작하거나 raw를 수정하지 않았다.

## 3. 측정 결과

- 실행 runtime-20261031-102334,정상 종료. root/dispatcher/clip 각 **104회**,post-stage 캐시 좌표 **401개**,native observer 오류/drop0.
- clip 시계 **0.648535배**,root와 clip 증가량 잔차0. 새 영상은75644630bytes이며 최종 해시를 기록했다.
- bridge **38ACK**의 frame/T/A가 CPU TSV와 정확히 일치. offset 허용폭 **14.2873ms**,epoch spread **7.7975ms**. 절대 QPC 동일성 검사는 실패 상태로 유지하며 원점 변환 검증과 구분한다.
- 영상540frame 중 **484개 HUD 정확 일치**,56개 제외. CPU/화면353쌍,변화량 상관 **0.946186**.
- 추가100ms 여유를 쓰는 기존 매칭 방식은399표본/297frame을 연결했으나102frame에 서로 다른 native 상태가 섞였다. 미래/다른 시점의 반복 포즈를 동일 frame 증거로 수용하지 않았다.
- 추가 여유0ms의 ACK 시간 범위에서는 **296표본/296frame**만 유일하게 일치.25ms 여유에서도 같은 선택이다.104개 불일치와1개 모호 표본은 제외했다. 연속 native 상태192쌍 중 영상까지 연결된 것은 **144쌍**이다.
- 불일치104개는 모두 **accumulator=0** 표본. positive accumulator는296개 일치,1개 모호.144쌍에는 새 clip sample/reset 경계 **0개**다. 따라서 목표 경계를 아직 연결하지 못했다.
- mission15~55초 p50/p95/p99 **20.0/35.0/37.0ms**. 녹화/훅 비용 포함이며64성능 또는 최적화 개선으로 사용하지 않는다.

## 4. 원인 분석

확인: 누적기가 양수인 구간에서는 native stage의 캐시 좌표와 CPU/영상 포즈를 부분 연결했다. 반면 새로운 평가 뒤0으로 초기화된 경계에서는 같은 방식이 성립하지 않았다. 넓은 시간 여유와 위치 지문만으로 reset 순간의 frame identity를 선언하면 오판할 수 있다.144쌍 수용이 목표 경계의 성공을 의미하지 않는다.

정적 선택: b13380는 renderer+a0를 b13410에 전달하고,b13410의 해당 경로는 renderer+190 skeleton의 dirtyA8을1로 표시하며+30의 local TRS 배열에 접근한다. 이는 선택 명령의 dataflow이며 모든 함수/virtual effect를 복원한 것은 아니다. query188+38 행렬 캐시의 신선도나 renderer+190과의 현재 alias 관계를 이번 native observer는 기록하지 않았다.

가설: 평가/보간 세대 전환에서 캐시 행렬·새 TRS·CPU getter가 서로 다른 시점의 값을 보일 수 있다. 정상적인 lazy cache와 실제 시각 불연속을 아직 구분하지 못했다. 이 현상을 곧바로 엔진 버그 또는 확정된 끊김 원인으로 부르지 않는다. 느린 clip 시계 증거는 유지되지만 reset 경계의 표시 인과는 미확인이다.

## 5. 검증 수준

native observer/게임·녹화 종료/독립 좌표 매핑/38ACK offset: 진단 성공. bridge producer의 절대원점 assertion: 실패,오프라인 변환 검증과 별도 표기. native/CPU/영상144쌍: 부분 진단. reset 경계/캐시 신선도/최종 GPU 표시: 미확인. 수정 효과·반복 기능 회귀·64비용·20ms p95·전체 수용: 미완료.

## 6. 변경 사항

빌드 B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185 유지,새 PBO/서명/배포 없음. 실행 전3개 의존 PBO의 기존 서명 검증 성공. 새 entry observer/driver·bridge·정적 선택·분석/보고서만 추가했고 공용 D61D·원시 기록·운영 서버·개인키는 보존했다. 이전 관찰기와 보고서를 덮어쓰지 않았다. OBS 출력 위치 변경 전 설정도 보존했다.

## 7. 원시 증거

plan/preflight/six-point-calibration/post-stage-static/interpolated-trs-writer-selection/bridge stderr/clock-bridge-calibration/native-pose-join/native-pose-strict/native-pose-margin25/joined-summary/results를 연결했다. profile의 raw calls/status/configuration/launch/native-clock-bridge/visible-frame-clock/HUD/윤곽/포즈 분석과 finalized MKV를 보존했다. 새 observer의 source hash 및 분석 입력 hash를 대조했고 checkpoint 저장 때 재검증한다. 공개 자료는 요약·선정 표본·선정 함수이며 raw 전체·영상·로컬 경로·프로세스/스레드/실행 포인터·개인키는 제외한다.

## 8. 잔여 문제

다음 observer는 **dirtyA8,query188/interpolated190 alias,실제 보간 local TRS(+30)**를 함께 기록해0누적기 경계를 확인해야 한다. 기존 cached matrix만 다시 같은 방식으로 읽는 실험은 반복하지 않는다. 실제 matrix refresh 계약과 출력 소유가 확인되면 같은 영상의 reset frame을 연결한다. getter를 강제 호출해 캐시를 갱신하면 관찰 대상이 바뀌므로 우선 passive 데이터와 원본 함수 순서를 사용한다.

지원되는 평가/시간 전달 계약이 확보되기 전 새 보간·float dt주입·고정 playback 배수는 적용하지 않는다.64 동적 혼잡 재현/기능 회귀와 비용 귀속은 각각 별도 조건으로 계속 관리한다. 이 실험의 partial join이나 bridge 보정만으로 애니메이션 개선을 선언하지 않는다.

선정 공개 본문. 로컬 보고서 SHA-256: `b591fee43ec67d8557dae56348ce79ac250f089dbd12abc0af02d33b781b6add`. 원시 전체가 아니다.
