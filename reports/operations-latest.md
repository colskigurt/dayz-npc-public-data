# 역순 반복: 잔여시간 보존 clock 효과 재현, 프레임별 시간 정렬은 미해결

작성: 2026-10-31T11:33:27.531134+09:00

## 1. 작업 목표

앞선 잔여시간 보존 인과시험을 역순으로 반복해 실행 순서에 의존한 결과인지 확인한다. 느린 총 재생 속도와 frame별 불균등한 자세 변화를 분리한다. 기존 NPC·정상 낙하·복구·운영 서버를 보존하며64기능/성능 시험을 이 실험으로 대체하지 않는다.

## 2. 실제 수행

앞선 A→B에 이어 같은 signed B8FB·미션·그래픽·capture layout·run2·VSync·query 조건에서 B→A를 실행했다. A는 원래 scheduler, B는 고정50ms 평가 후 잔여시간을 accumulator와 descriptor numerator에 보존하는 격리 후보다. 논리 NPC1명/native8파트이며8명 이동 시험이 아니다. 조건당 총2회다. 모델·driver·관찰기를 그대로 사용했고 candidate는 기존 baseline admission/source hash gate를 통과했다. 관찰30초·창 녹화45초·미션120초 후 정상 종료했다.

실행 전3개 기존 PBO 서명 검증을 각각 통과했다. 컴퓨터 유즈로 게임 창과 actor/HUD를 확인하고4시점 녹화 이미지를 확인했다. 영상5/12/20초부터 각3초를 독립 HUD와 윤곽 분석으로 연결했다. 미확인 HUD·비연속 frame·활성 native 구간 밖 표본은 제외했다. 사후 분석으로 graphClock+numerator의 frame별 증가와 현재/인접 script interval의 관계를 비교했다. 분석 때문에 실행 중 시험을 중단하지 않았다.

## 3. 측정 결과

| 실행 | 조건 | clock/실제 시간 | source-pose 추정/mission | 파트당 평가 | 유효 CPU/영상 쌍 | 변화량 상관 |
|---|---|---:|---:|---:|---:|---:|
|runtime-20261031-110449|baseline30|0.659440729|0.657745777|395|437|0.943663|
|runtime-20261031-111204|candidate30|1.000003060|0.999110643|600|431|0.915311|
|runtime-20261031-112434|candidate30|0.999969805|0.998998030|600|381|0.918593|
|runtime-20261031-112900|baseline30|0.653588028|0.653220423|393|420|0.943394|

후보 두 실행은 사전 clock 허용오차±0.005를 통과했다. 각4800회 native 숫자 변경,8파트 clock/numerator 최대 spread0, 평가 여부 불일치0, long-frame guard0, observer/drop 오류0을 확인했다. 각 조건의 두 실행 수치는 JSON에 보존했다. 후보의 평가 횟수 증가를 성능 개선으로 해석하지 않는다. 최종1개체 mission p50/p95/p99와 observer read bracket은 comparison의 활성 구간 수치를 따른다. GPU Present 또는64개체 수치는 아니다.

시간 정렬 탐색 결과:
- runtime-20261031-110449: baseline30, effective step 대 현재 interval 상관 -0.502595, 직전 interval 상관 0.496178, 직전 interval RMS 잔차 14.084617ms.
- runtime-20261031-111204: candidate30, effective step 대 현재 interval 상관 -0.986313, 직전 interval 상관 0.998430, 직전 interval RMS 잔차 0.705176ms.
- runtime-20261031-112900: baseline30, effective step 대 현재 interval 상관 -0.533232, 직전 interval 상관 0.528710, 직전 interval RMS 잔차 14.276482ms.
- runtime-20261031-112434: candidate30, effective step 대 현재 interval 상관 -0.986431, 직전 interval 상관 0.998347, 직전 interval RMS 잔차 0.728186ms.

영상의 단순 counter-signature heuristic이 false인 구간도 보존했다. 이는 HUD의 정확한 frame/T/A 식별과 다른 거친 검사다. heuristic을 수용 근거로 사용하지 않았고 exact matching이 실패한 표본은 제외했다. 후보 첫 구간은148/180으로 식별률이 낮았으며 추가 두 구간은179/180씩이었다. 원시 영상과 제외 기준을 남긴다.

## 4. 원인 분석

확인: 실행 순서를 뒤집어도 잔여시간 보존의 총 clock·독립 source-pose 진행 효과가 재현됐다. 이 고정 run 조건에서 초과 누적시간 reset이 느린 재생에 기여한다는 진단을 강화한다. native8파트는 같은 시간에 평가·보간됐다.

추정/미확인: frame별 시간 증가가 이전 업데이트 간격과 강하게 연결될 경우 총 재생속도 보정 후에도 현재 표시 간격과 불일치할 수 있다. 인접 interval은 서로 상관되고 source FK·윤곽은 proxy이므로 상관만으로 유일한 원인을 확정하지 않는다. 실제 incoming dt 전달과 최종 GPU 소비 순서를 더 확인해야 한다.

지지되지 않은 설명과 한계: 네 run의 source-phase 음수 step은0이다. 역행이 이번 주원인이라는 증거는 없다. body hold가 거의 없더라도 큰 step은 남아 있고, 재생속도 증가가 step 크기를 키우므로 단일 tail 수치만으로 악화/개선을 확정하지 않는다. 정지 프레임 수 감소를 부드러움 완성으로 보고하지 않는다.

## 5. 검증 수준

- 고정1개체 run2의 잔여시간 보존 clock 효과: 반복 검증 완료, A/B/B/A·조건당2회, 같은 환경 한정.
- CPU/native/실제 영상 시간 연결: 진단 성공. 지각적 부드러움·최종 GPU phase는 미수용.
- 시간 정렬 offset: 사후 탐색 분석이며 인과 확정 아님.
- 지원되는 배포 수정·14clip 전체·8/64기능 회귀·혼잡/등반 종료 계약·64비용 귀속·20ms p95·최종 수용: 미완료.

## 6. 변경 사항

빌드 B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185와 공용 배포본 유지. 새 PBO/재서명/배포 없음, 기존3개 PBO 서명 검증 성공. 별도 반복 폴더에 watch/bridge/분석을 파생했고 실제 실행 순서와 유동 평가 횟수를 반영한 비교 도구,4run 집계 및 시간 정렬 분석을 추가했다. 후보는 격리 프로세스의 accumulator/descriptor 숫자를 변경한다. duration·buffer 포인터·원본 EXE/PBO를 바꾸는 구현이 아니며 지원된 모드 배포 방식이 확보됐다는 뜻도 아니다. 원본 보고서·raw·운영 서버·개인키는 보존했다.

## 7. 원시 증거

repeated-results와comparison-evidence가 네 run을 연결한다. attach-readiness, status/config/source, calls, ACK, CPU TSV, source fit/pairs, finalized MKV, HUD·윤곽·pose alignment, temporal-alignment와 분석 원문을 보존한다. 정상 종료와 기존 hash/파일 안정성을 검사한 뒤 기존 checkpoint·TXT·B안에 게시한다. 공개본은 선정 집계·모델/분석 관련 텍스트이며 전체 raw/영상·주소·로컬 경로·프로세스/개인키는 제외한다. 공개 HTTP 성공과 외부 AI 재접근은 별도로 판정한다.

## 8. 잔여 문제

다음은 실제 incoming dt와 CPU 조회/표시 순서 계약을 대조하고, 지원 API/그래프 자산으로 같은 시간 보존 효과를 적용할 경로를 조사하는 것이다. 이미 반려한 fraction-only/무근거 duration 배속은 반복하지 않는다. clock만 맞추는 추가 보간을 바로 도입하지 않는다. 긴 frame guard가 이번에는 발동하지 않았으므로 backlog 안정성은 미검증이다.

기능 축은 별도로 B8FB 실제8→64 재접근·원래 actor61 혼잡·등반 종료·정상 낙하 회귀를 마쳐야 한다. 성능 축은64개체 비용 귀속을 대조 가능한 조건으로 측정해야 한다. 두 축을1개체 애니메이션 결과로 통과 처리하지 않는다. 원인 귀속을 흐리는 새 군집 설계/자산 추가는 보류하고, 필요한 추가 도구·사용자 관찰은 구체적인 검증 질문과 함께 요청한다.

선정 공개 본문. 로컬 보고서 SHA-256: `0784ddb39fb2bc0317df347133008995118a0192ec195c6e908357d692132a9a`. 원시 전체가 아니다.
